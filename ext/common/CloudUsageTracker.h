/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_CLOUD_USAGE_TRACKER_H_
#define _PASSENGER_CLOUD_USAGE_TRACKER_H_

#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <vector>
#include <utility>

#include <cstdio>
#include <ctime>
#include <cerrno>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include <curl/curl.h>

#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <oxt/thread.hpp>
#include <oxt/system_calls.hpp>
#include <oxt/backtrace.hpp>
#include <Logging.h>
#include <Exceptions.h>
#include <FileDescriptor.h>
#include <Utils.h>
#include <Utils/json.h>
#include <Utils/IOUtils.h>
#include <Utils/StrIntUtils.h>
#include <Utils/SystemTime.h>
#include <Utils/License.h>
#include <Utils/Curl.h>

namespace Passenger {

using namespace std;
using namespace oxt;


class CloudUsageTracker {
protected:
	struct UsagePointComparator {
		bool operator()(const string &a, const string &b) const {
			return atoll(a.c_str()) < atoll(b.c_str());
		}
	};

	typedef vector< pair<const char *, string> > MachineProperties;

	string datadir;
	string url;
	string certificate;
	string hostname;
	bool machinePropertiesDetected;
	MachineProperties savedMachineProperties;
	CurlProxyInfo proxyInfo;
	oxt::thread *thr;
	char lastErrorMessage[CURL_ERROR_SIZE];

	void abortWithError(const string &message) {
		if (abortHandler != NULL) {
			abortHandler(message);
		} else {
			P_ERROR(message);
			abort();
		}
	}

	void recordUsagePoint() {
		TRACE_POINT();
		time_t now = SystemTime::get();
		string filename = datadir + "/" + toString(now);
		P_DEBUG("Recording usage point: " << filename);
		FILE *f = fopen(filename.c_str(), "w");
		if (f == NULL) {
			stringstream message;
			message << "Cannot write to the Phusion Passenger Enterprise " <<
				"cloud licensing data directory (" << datadir << "). Please " <<
				"ensure that that directory exists and is writable by user '" <<
				getProcessUsername() << "'.";
			abortWithError(message.str());
		}
		setvbuf(f, NULL, _IOFBF, 0);
		fprintf(f, "%lld\n", (long long) now);
		if (flushFile(f) != 0) {
			int e = errno;
			fclose(f);
			unlink(filename.c_str());
			stringstream message;
			message << "An I/O error occurred while recording a cloud usage point: " <<
				strerror(errno) << " (errno=" << e << ")";
			abortWithError(message.str());
		} else {
			fsync(fileno(f));
			fclose(f);
		}
	}

	vector<string> listUsagePoints() {
		TRACE_POINT();
		DIR *dir = opendir(datadir.c_str());
		if (dir == NULL) {
			stringstream message;
			message << "Cannot read the Phusion Passenger Enterprise " <<
				"cloud licensing data directory (" << dir << "). Please " <<
				"ensure that that directory exists and is readable by user '" <<
				getProcessUsername() << "'.";
			abortWithError(message.str());
		}

		UPDATE_TRACE_POINT();
		vector<string> result;
		struct dirent *ent;
		bool noerror = true;
		while ((ent = readdir(dir)) != NULL && noerror) {
			if (ent->d_name[0] == '.') {
				continue;
			}

			UPDATE_TRACE_POINT();
			result.push_back(ent->d_name);
		}

		closedir(dir);
		std::sort(result.begin(), result.end(), UsagePointComparator());
		return result;
	}

	unsigned int sendUsagePoints(const vector<string> &usagePoints) {
		TRACE_POINT();
		unsigned int result = 0;

		foreach (const string &name, usagePoints) {
			UPDATE_TRACE_POINT();
			string filename = datadir + "/" + name;
			string content;
			P_DEBUG("Sending usage point " << filename << " to " << url);
			try {
				content = readAll(filename);
			} catch (const SystemException &e) {
				if (e.code() == ENOENT) {
					// Ignore, file has been removed while iterating.
				} else {
					throw;
				}
			}

			UPDATE_TRACE_POINT();
			if (sendUsagePoint(content)) {
				result++;
				unlink(filename.c_str());
			} else {
				P_DEBUG("Could not send usage point");
				break;
			}
		}
		P_DEBUG("Sent " << result << " usage points in this cycle");
		return result;
	}

	static size_t curlDataReceived(void *buffer, size_t size, size_t nmemb, void *userData) {
		string *str = (string *) userData;
		str->append((const char *) buffer, size * nmemb);
		return size * nmemb;
	}

	static bool validateResponse(const Json::Value &response) {
		if (response.isObject() && response["status"].isString()) {
			string status = response["status"].asString();
			if (status == "ok") {
				return true;
			} else if (status == "error") {
				return response["message"].isString();
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	bool sendUsagePoint(const string &content) {
		TRACE_POINT();
		string responseData;
		CURLcode code = performCurlAction(autodetectMachineProperties(), content, responseData);

		if (code == 0) {
			Json::Reader reader;
			Json::Value response;

			if (!reader.parse(responseData, response, false) || !validateResponse(response)) {
				P_WARN("Could not contact the Phusion Passenger Enterprise cloud licensing server "
					"(parse error: " << reader.getFormattedErrorMessages().c_str() << "; data: \"" <<
					cEscapeString(responseData) << "\"). This problem may be temporary, "
					"but if it persists for more than 3 days then Phusion Passenger Enterprise "
					"will be disabled until the cloud licensing server could be contacted again. "
					"To ensure proper access to the cloud licensing server, please try these:\n"
					"- Ensure that your network connection to https://www.phusionpassenger.com works.\n"
					"- If you can only access https://www.phusionpassenger.com via a proxy, "
					"please set the config option 'PassengerCtl cloud_licensing_proxy PROXY_URL' (Apache) "
					"or 'passenger_ctl cloud_licensing_proxy PROXY_URL' (Nginx). 'PROXY_URL' takes the format of "
					"protocol://username:password@hostname:port, where 'protocol' is either 'http' or 'socks5'.\n"
					"Please contact support@phusion.nl if you require any assistance.");
				return false;
			} else if (response["status"].asString() == "ok") {
				return true;
			} else {
				// response == error
				stringstream message;
				message << "There is a problem with your Phusion Passenger Enterprise "
					"cloud license. Phusion Passenger Enterprise has been disabled until "
					"this problem is resolved. Please contact support@phusion.nl if you "
					"require assistance. The problem is as follows: " <<
					response["message"].asString();
				abortWithError(message.str());
				return false; // Not reached; avoid compiler warning.
			}
		} else {
			P_WARN("Could not contact the Phusion Passenger Enterprise cloud licensing server "
				"(HTTP error: " << lastErrorMessage << "). This problem may be temporary, "
				"but if it persists for more than 3 days then Phusion Passenger Enterprise "
				"will be disabled until the cloud licensing server could be contacted again. "
				"To ensure proper access to the cloud licensing server, please try these:\n"
				"- Ensure that your network connection to https://www.phusionpassenger.com works.\n"
				"- If you can only access https://www.phusionpassenger.com via a proxy, "
				"please set the config option 'PassengerCtl cloud_licensing_proxy PROXY_URL' (Apache) "
				"or 'passenger_ctl cloud_licensing_proxy PROXY_URL' (Nginx). 'PROXY_URL' takes the format of "
				"protocol://username:password@hostname:port, where 'protocol' is either 'http' or 'socks5'.\n"
				"Please contact support@phusion.nl if you require any assistance.");
			return false;
		}
	}

	const MachineProperties &autodetectMachineProperties() {
		TRACE_POINT();
		if (!machinePropertiesDetected) {
			P_DEBUG("Autodetecting machine properties");
			autodetectAmazonInstanceType(savedMachineProperties);
			machinePropertiesDetected = true;
			P_DEBUG("Machine properties autodetection finished");
		}
		return savedMachineProperties;
	}

	static void autodetectAmazonInstanceType(MachineProperties &properties) {
		TRACE_POINT();
		P_DEBUG("Autodetecting Amazon instance type");
		string responseData;
		char lastErrorMessage[CURL_ERROR_SIZE];
		CURL *curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, "http://169.254.169.254/latest/meta-data/instance-type");
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, lastErrorMessage);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlDataReceived);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if (code == 0) {
			long responseCode;

			if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode) != CURLE_OK) {
				P_ERROR("Cannot not autodetect Amazon instance type (internal error: could "
					"not query libcurl response code). Assuming this is not an Amazon instance");
				return;
			}

			if (responseData.empty()) {
				P_ERROR("Cannot not autodetect Amazon instance type (HTTP error: the server "
					"returned an empty response). Assuming this is not an Amazon instance");
				return;
			}

			properties.push_back(make_pair("aws_instance_type", responseData));
		} else {
			P_DEBUG("Cannot contact Amazon metadata server (HTTP error: " << lastErrorMessage <<
				"). Assuming this is not an Amazon instance");
		}
	}

	// Virtual so that unit tests can stub it.
	virtual CURLcode performCurlAction(const MachineProperties &properties,
		const string &content, string &responseData)
	{
		TRACE_POINT();
		CURL *curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 180);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, lastErrorMessage);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlDataReceived);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
		if (certificate.empty()) {
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		} else {
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
			curl_easy_setopt(curl, CURLOPT_CAINFO, certificate.c_str());
		}
		setCurlProxy(curl, proxyInfo);

		struct curl_httppost *post = NULL;
		struct curl_httppost *last = NULL;
		string now = toString(SystemTime::get());
		
		curl_formadd(&post, &last,
			CURLFORM_PTRNAME, "content",
			CURLFORM_PTRCONTENTS, content.data(),
			CURLFORM_CONTENTSLENGTH, (long) content.size(),
			CURLFORM_END);
		curl_formadd(&post, &last,
			CURLFORM_PTRNAME, "license_key",
			CURLFORM_PTRCONTENTS, licenseKey,
			CURLFORM_CONTENTSLENGTH, (long) strlen(licenseKey),
			CURLFORM_END);
		curl_formadd(&post, &last,
			CURLFORM_PTRNAME, "hostname",
			CURLFORM_PTRCONTENTS, hostname.data(),
			CURLFORM_CONTENTSLENGTH, (long) hostname.size(),
			CURLFORM_END);
		curl_formadd(&post, &last,
			CURLFORM_PTRNAME, "time",
			CURLFORM_PTRCONTENTS, now.data(),
			CURLFORM_CONTENTSLENGTH, (long) now.size(),
			CURLFORM_END);
		
		MachineProperties::const_iterator it;
		for (it = properties.begin(); it != properties.end(); it++) {
			const pair<const char *, string> &p = *it;
			curl_formadd(&post, &last,
				CURLFORM_PTRNAME, p.first,
				CURLFORM_PTRCONTENTS, p.second.data(),
				CURLFORM_CONTENTSLENGTH, (long) p.second.size(),
				CURLFORM_END);
		}
		
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		CURLcode code = curl_easy_perform(curl);
		curl_formfree(post);
		curl_easy_cleanup(curl);

		return code;
	}

	// Stubable by unit tests.
	virtual int flushFile(FILE *f) const {
		return fflush(f);
	}

	void threadMain() {
		TRACE_POINT();
		while (!this_thread::interruption_requested()) {
			UPDATE_TRACE_POINT();
			runOneCycle();
			UPDATE_TRACE_POINT();
			syscalls::usleep(15 * 60 * 1000000);
		}
	}

public:
	unsigned int threshold;
	function<void (const string &message)> abortHandler;

	CloudUsageTracker(const string &dir,
		const string &baseUrl = string(),
		const string &_certificate = string(),
		const string &proxyAddress = string())
		: datadir(dir),
		  certificate(_certificate),
		  threshold(3 * 24 * 4)
	{
		long size = sysconf(_SC_HOST_NAME_MAX) + 1;
		char buf[size];
		memset(buf, 0, size);
		if (gethostname(buf, size) == 0) {
			hostname = buf;
		} else {
			int e = errno;
			throw SystemException("Cannot query the host name", e);
		}
		machinePropertiesDetected = false;

		try {
			proxyInfo = prepareCurlProxy(proxyAddress);
		} catch (const ArgumentException &e) {
			throw RuntimeException("Invalid cloud license tracking proxy address \"" +
				proxyAddress + "\": " + e.what());
		}
		thr = NULL;

		url = baseUrl.empty()
			? "https://www.phusionpassenger.com"
			: baseUrl;
		url += "/cloud_service/track_usage";
	}

	~CloudUsageTracker() {
		if (thr != NULL) {
			thr->join();
			delete thr;
		}
	}

	void start() {
		thr = new oxt::thread(
			boost::bind(&CloudUsageTracker::threadMain, this),
			"Cloud usage tracker",
			1024 * 512
		);
	}

	void runOneCycle() {
		TRACE_POINT();
		P_DEBUG("Begin tracking cloud usage cycle");
		recordUsagePoint();
		vector<string> usagePoints = listUsagePoints();
		unsigned int sent = sendUsagePoints(usagePoints);
		if (usagePoints.size() - sent > threshold) {
			abortWithError("Phusion Passenger Enterprise hasn't been able to contact "
				"the cloud licensing server (https://www.phusionpassenger.com) for "
				"more than 3 days.\n"
				"- Please ensure that your network connection to https://www.phusionpassenger.com works.\n"
				"- If you can only access https://www.phusionpassenger.com via a proxy, "
				"please set the config option 'PassengerCtl cloud_licensing_proxy PROXY_URL' (Apache) "
				"or 'passenger_ctl cloud_licensing_proxy PROXY_URL' (Nginx). 'PROXY_URL' takes the format of "
				"protocol://username:password@hostname:port, where 'protocol' is either 'http' or 'socks5'.\n"
				"If the problem persists, please contact support@phusion.nl.");
		}
		P_DEBUG("Done tracking cloud usage cycle");
	}
};


} // namespace Passenger

#endif /* _PASSENGER_CLOUD_USAGE_TRACKER_H_ */
