/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
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
#include <limits.h>

#include <curl/curl.h>

#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <oxt/thread.hpp>
#include <oxt/system_calls.hpp>
#include <oxt/backtrace.hpp>
#include <jsoncpp/json.h>
#include <Logging.h>
#include <Exceptions.h>
#include <FileDescriptor.h>
#include <Utils.h>
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
	string hostname;
	bool sslCertCheck;
	bool machinePropertiesDetected;
	MachineProperties savedMachineProperties;
	CurlProxyInfo proxyInfo;
	oxt::thread *thr;
	char lastErrorMessage[CURL_ERROR_SIZE];

	void handleLicenseError(const string &message) {
		TRACE_POINT();
		if (licenseErrorHandler != NULL) {
			licenseErrorHandler(message);
		} else {
			P_ERROR(message);
			sendEmailToRoot(message);
		}
	}

	void sendEmailToRoot(const string &message) {
		string recipients = "root";
		string currentUser = getProcessUsername(false);
		if (!currentUser.empty() && currentUser != "root") {
			recipients.append(",");
			recipients.append(currentUser);
		}

		P_INFO("Sending email about licensing problem to the following user(s): " + recipients);
		try {
			bool sent = sendEmailToRootUsingMail(recipients, message);
			if (!sent) {
				P_WARN("Could not send email using 'mail'. Trying again using 'sendmail'...");
				sent = sendEmailToRootUsingSendmail(recipients, message);
				if (!sent) {
					P_WARN("Could not send email using 'sendmail'. Giving up");
				}
			}
			if (sent) {
				P_INFO("Email successfully sent");
			}
		} catch (const RuntimeException &e) {
			P_WARN(e.what());
		}
	}

	bool sendEmailToRootUsingMail(const string &recipients, const string &message) {
		TRACE_POINT();
		char path[PATH_MAX] = "/tmp/passenger-email.XXXXXXXX";
		int fd = mkstemp(path);
		if (fd != -1) {
			try {
				writeExact(fd, P_STATIC_STRING("\n"));
				writeExact(fd, message);
				writeExact(fd, P_STATIC_STRING("\n.\n"));
			} catch (const SystemException &e) {
				P_ERROR("Cannot write to " << path << ": " << e.what());
				close(fd);
				return false;
			}
			close(fd);

			bool result = runShellCommand("env PATH=/usr/sbin:$PATH mail "
				"-s 'Phusion Passenger Enterprise cloud licensing problem' "
				"'" + recipients + "' < '" +
				string(path) + "'") == 0;
			unlink(path);
			return result;
		} else {
			throw RuntimeException("Could not send email: unable to create a temp file");
		}
	}

	bool sendEmailToRootUsingSendmail(const string &recipients, const string &message) {
		TRACE_POINT();
		char path[PATH_MAX] = "/tmp/passenger-email.XXXXXXXX";
		int fd = mkstemp(path);
		if (fd != -1) {
			try {
				writeExact(fd, P_STATIC_STRING("To: "));
				writeExact(fd, recipients);
				writeExact(fd, P_STATIC_STRING("\n"
					"Subject: Phusion Passenger Enterprise cloud licensing problem\n"
					"\n"));
				writeExact(fd, message);
				writeExact(fd, P_STATIC_STRING("\n.\n"));
			} catch (const SystemException &e) {
				P_ERROR("Cannot write to " << path << ": " << e.what());
				close(fd);
				return false;
			}
			close(fd);

			bool result = runShellCommand("env PATH=/usr/sbin:$PATH sendmail -t < '" +
				string(path) + "'") == 0;
			unlink(path);
			return result;
		} else {
			throw RuntimeException("Could not send email: unable to create a temp file");
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
				"licensing data directory (" << datadir << "). Please " <<
				"ensure that that directory exists and is writable by user '" <<
				getProcessUsername() << "'.";
			throw RuntimeException(message.str());
		}
		setvbuf(f, NULL, _IOFBF, 0);
		fprintf(f, "%lld\n", (long long) now);
		if (flushFile(f) != 0) {
			int e = errno;
			fclose(f);
			unlink(filename.c_str());
			stringstream message;
			message << "An I/O error occurred while recording a usage point: " <<
				strerror(errno) << " (errno=" << e << ")";
			throw RuntimeException(message.str());
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
				"licensing data directory (" << dir << "). Please " <<
				"ensure that that directory exists and is readable by user '" <<
				getProcessUsername() << "'.";
			throw RuntimeException(message.str());
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
				P_WARN("Could not contact the Phusion Passenger Enterprise licensing server "
					"(parse error: " << reader.getFormattedErrorMessages().c_str() << "; data: \"" <<
					cEscapeString(responseData) << "\"). To ensure proper access to the licensing " <<
					"server, please try these:\n"
					"- Ensure that your network connection to https://www.phusionpassenger.com works.\n"
					"- If you can only access https://www.phusionpassenger.com via a proxy, "
					"please set the config option 'PassengerCtl licensing_proxy PROXY_URL' (Apache) "
					"or 'passenger_ctl licensing_proxy PROXY_URL' (Nginx). 'PROXY_URL' takes the format of "
					"protocol://username:password@hostname:port, where 'protocol' is either 'http' or 'socks5'.\n"
					"Please contact support@phusion.nl if you require any assistance.");
				return false;
			} else if (response["status"].asString() == "ok") {
				return true;
			} else {
				// response == error
				stringstream message;
				message << "There is a problem with your Phusion Passenger Enterprise "
					"license. Please contact support@phusion.nl if you "
					"require assistance. The problem is as follows: " <<
					response["message"].asString();
				handleLicenseError(message.str());
				return false;
			}
		} else {
			P_WARN("Could not contact the Phusion Passenger Enterprise licensing server "
				"(HTTP error: " << lastErrorMessage << "). "
				"To ensure proper access to the licensing server, please try these:\n"
				"- Ensure that your network connection to https://www.phusionpassenger.com works.\n"
				"- If you can only access https://www.phusionpassenger.com via a proxy, "
				"please set the config option 'PassengerCtl licensing_proxy PROXY_URL' (Apache) "
				"or 'passenger_ctl licensing_proxy PROXY_URL' (Nginx). 'PROXY_URL' takes the format of "
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

	// Virtual so that unit tests can stub it.
	virtual bool autodetectAmazonInstanceType(MachineProperties &properties) const {
		TRACE_POINT();
		P_DEBUG("Autodetecting Amazon instance type");
		string responseData;
		char lastErrorMessage[CURL_ERROR_SIZE];
		CURL *curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, "http://169.254.169.254/2014-02-25/meta-data/instance-type");
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, lastErrorMessage);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlDataReceived);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
		CURLcode code = curl_easy_perform(curl);

		if (code == 0) {
			long responseCode;

			if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode) != CURLE_OK) {
				P_ERROR("Cannot not autodetect Amazon instance type (internal error: could "
					"not query libcurl response code). Assuming this is not an Amazon instance");
				curl_easy_cleanup(curl);
				return false;
			}

			curl_easy_cleanup(curl);

			// On some machines, we get a responseCode of 0 even though the response body
			// is correct. The reason is unknown.
			if (responseCode != 200 && responseCode != 0) {
				P_ERROR("Cannot not autodetect Amazon instance type (HTTP error: response "
					"code " << responseCode << "; body \"" << cEscapeString(responseData) <<
					"\"). Assuming this is not an Amazon instance");
				return false;
			}

			if (responseData.empty()) {
				P_ERROR("Cannot not autodetect Amazon instance type (HTTP error: the server "
					"returned an empty response). Assuming this is not an Amazon instance");
				return false;
			}

			P_DEBUG("Autodetected Amazon instance type: " << responseData);
			properties.push_back(make_pair("aws_instance_type", responseData));
			return true;
		} else {
			P_DEBUG("Cannot contact Amazon metadata server (HTTP error: " << lastErrorMessage <<
				"). Assuming this is not an Amazon instance");
			curl_easy_cleanup(curl);
			return false;
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
		if (!sslCertCheck) {
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
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
			try {
				runOneCycle();
			} catch (const tracable_exception &e) {
				P_ERROR(e.what() << "\n" << e.backtrace());
			}
			UPDATE_TRACE_POINT();
			syscalls::usleep(15 * 60 * 1000000);
		}
	}

public:
	unsigned int threshold;
	bool autoSend;
	boost::function<void (const string &message)> licenseErrorHandler;

	CloudUsageTracker(const string &dir,
		const string &baseUrl = string(),
		const string &proxyAddress = string(),
		bool _sslCertCheck = true,
		bool _autoSend = true)
		: datadir(dir),
		  sslCertCheck(_sslCertCheck),
		  threshold(3 * 24 * 4),
		  autoSend(_autoSend)
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
			throw RuntimeException("Invalid license tracking proxy address \"" +
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

	bool runOneCycle() {
		TRACE_POINT();
		bool result;
		P_DEBUG("Begin tracking usage cycle");
		recordUsagePoint();
		if (autoSend) {
			vector<string> usagePoints = listUsagePoints();
			unsigned int sent = sendUsagePoints(usagePoints);
			if (usagePoints.size() - sent > threshold) {
				handleLicenseError("Phusion Passenger Enterprise hasn't been able to contact "
					"the licensing server (https://www.phusionpassenger.com) for "
					"more than 3 days.\n"
					"- Please ensure that your network connection to https://www.phusionpassenger.com works.\n"
					"- If you can only access https://www.phusionpassenger.com via a proxy, "
					"please set the config option 'PassengerCtl licensing_proxy PROXY_URL' (Apache) "
					"or 'passenger_ctl licensing_proxy PROXY_URL' (Nginx). 'PROXY_URL' takes the format of "
					"protocol://username:password@hostname:port, where 'protocol' is either 'http' or 'socks5'.\n"
					"If the problem persists, please contact support@phusion.nl.");
			}
			result = usagePoints.size() - sent == 0;
		} else {
			P_DEBUG("Not auto-sending usage cycle");
			result = false;
		}
		P_DEBUG("Done tracking usage cycle");
		return result;
	}

	Json::Value dumpMachineProperties() {
		const MachineProperties &properties = autodetectMachineProperties();
		Json::Value doc(Json::objectValue);
		MachineProperties::const_iterator it;

		for (it = properties.begin(); it != properties.end(); it++) {
			const pair<const char *, string> &p = *it;
			doc[string(p.first)] = p.second;
		}
		return doc;
	}
};


} // namespace Passenger

#endif /* _PASSENGER_CLOUD_USAGE_TRACKER_H_ */
