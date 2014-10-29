#include <TestSupport.h>
#include <CloudUsageTracker.h>
#include <Constants.h>
#include <deque>
#include <sys/types.h>
#include <sys/stat.h>

using namespace Passenger;
using namespace std;

namespace tut {
	class TestTracker: public CloudUsageTracker {
	public:
		CURLcode defaultCurlCode;
		string defaultResponseData;
		deque<CURLcode> curlCodes;
		deque<string> responseDataList;
		vector<string> sentContent;

		int flushResult;

		TestTracker(const string &dir, bool autoSend)
			: CloudUsageTracker(dir, string(), string(), string(), autoSend),
			  defaultCurlCode(CURLE_OK),
			  defaultResponseData("{\"status\": \"ok\"}"),
			  flushResult(0)
			{ }

		virtual bool autodetectAmazonInstanceType(MachineProperties &properties) const {
			return false;
		}

		virtual CURLcode performCurlAction(const MachineProperties &properties,
			const string &content, string &responseData)
		{
			sentContent.push_back(content);

			if (responseDataList.empty()) {
				responseData = defaultResponseData;
			} else {
				responseData = responseDataList.front();
				responseDataList.pop_front();
			}

			CURLcode result;
			if (curlCodes.empty()) {
				result = defaultCurlCode;
			} else {
				result = curlCodes.front();
				curlCodes.pop_front();
			}
			snprintf(lastErrorMessage, sizeof(lastErrorMessage), "code %d", (int) result);
			return result;
		}

		virtual int flushFile(FILE *f) const {
			if (flushResult == 0) {
				return fflush(f);
			} else {
				errno = EIO;
				return flushResult;
			}
		}

		unsigned int sentCount() const {
			return sentContent.size();
		}
	};

	class AbortException: public RuntimeException {
	public:
		AbortException(const string &msg)
			: RuntimeException(msg)
			{ }
	};

	struct CloudUsageTrackerTest {
		TempDir datadirGuard;
		boost::shared_ptr<TestTracker> tracker;

		CloudUsageTrackerTest()
			: datadirGuard("tmp.clouddata")
		{
			SystemTime::force(1000);
		}

		~CloudUsageTrackerTest() {
			SystemTime::releaseAll();
			setLogLevel(DEFAULT_LOG_LEVEL);
		}

		void init(const string &datadir = "tmp.clouddata", bool autoSend = true) {
			tracker = boost::make_shared<TestTracker>(datadir, autoSend);
			tracker->licenseErrorHandler = licenseErrorHandler;
		}

		static void licenseErrorHandler(const string &message) {
			P_DEBUG(message);
			throw AbortException(message);
		}
	};

	#define DONT_RUN_WHEN_ROOT() do { \
			if (geteuid() == 0) { \
				return; \
			} \
		} while (false)


	DEFINE_TEST_GROUP(CloudUsageTrackerTest);

	TEST_METHOD(1) {
		set_test_name("On every cycle, it records a usage point and sends it immediately.");

		init();
		tracker->runOneCycle();

		ensure_equals("All usage points have been sent", listDir("tmp.clouddata").size(), 0u);
		ensure_equals("1 usage point was sent", tracker->sentCount(), 1u);
		ensure_equals(tracker->sentContent[0], "1000\n");
	}

	TEST_METHOD(2) {
		set_test_name("It aborts if it cannot create a file in the data directory while recording a usage point");
		DONT_RUN_WHEN_ROOT();

		init();
		chmod("tmp.clouddata", 0000);
		try {
			tracker->runOneCycle();
			fail("RuntimeException expected");
		} catch (const RuntimeException &e) {
			ensure(StaticString(e.what()).find("Cannot write to the Phusion Passenger Enterprise licensing data directory")
				!= string::npos);
		}
		chmod("tmp.clouddata", 0600);
		ensure_equals(listDir("tmp.clouddata").size(), 0u);
		ensure_equals(tracker->sentCount(), 0u);
	}

	TEST_METHOD(3) {
		set_test_name("It aborts if an I/O error occured while writing to a file in the data directory");

		init();
		tracker->flushResult = EOF;
		try {
			tracker->runOneCycle();
			fail("RuntimeException expected");
		} catch (const RuntimeException &e) {
			ensure(StaticString(e.what()).find("An I/O error occurred while recording a usage point")
				!= string::npos);
		}
		ensure_equals(listDir("tmp.clouddata").size(), 0u);
		ensure_equals(tracker->sentCount(), 0u);
	}

	TEST_METHOD(4) {
		set_test_name("Leaves a recorded usage point in the data directory if the license server cannot be connected to");

		init();
		setLogLevel(LVL_ERROR);
		tracker->curlCodes.push_back(CURLE_COULDNT_CONNECT);
		tracker->runOneCycle();
		vector<string> files = listDir("tmp.clouddata");
		ensure_equals(files.size(), 1u);
		ensure_equals(files[0], "1000");
		ensure_equals(readAll("tmp.clouddata/1000"), "1000\n");
	}

	TEST_METHOD(5) {
		set_test_name("Leaves a recorded usage point in the data directory if the license server returns an invalid response");

		init();
		setLogLevel(LVL_ERROR);
		tracker->responseDataList.push_back("Something went wrong");
		tracker->runOneCycle();
		vector<string> files = listDir("tmp.clouddata");
		ensure_equals(files.size(), 1u);
		ensure_equals(files[0], "1000");
		ensure_equals(readAll("tmp.clouddata/1000"), "1000\n");
	}

	TEST_METHOD(6) {
		set_test_name("Deletes the recorded usage point if the license server returns an ok response");

		init();
		tracker->responseDataList.push_back("{\"status\": \"ok\"}");
		tracker->runOneCycle();
		ensure_equals(listDir("tmp.clouddata").size(), 0u);
	}

	TEST_METHOD(7) {
		set_test_name("Leaves the recorded usage point in the data directory and aborts if the license server returns an error response");

		init();
		tracker->responseDataList.push_back("{\"status\": \"error\", \"message\": \"lp0 on fire\"}");
		try {
			tracker->runOneCycle();
			fail("AbortException expected");
		} catch (const AbortException &e) {
			ensure(string(e.what()).find("lp0 on fire") != string::npos);
		}
		vector<string> files = listDir("tmp.clouddata");
		ensure_equals(files.size(), 1u);
		ensure_equals(files[0], "1000");
	}

	TEST_METHOD(8) {
		set_test_name("It sends all previously recorded usage points");

		createFile("tmp.clouddata/100", "100\n");
		createFile("tmp.clouddata/101", "101\n");
		createFile("tmp.clouddata/102", "102\n");
		init();
		tracker->runOneCycle();
		ensure_equals(tracker->sentCount(), 4u);
		ensure_equals(tracker->sentContent[0], "100\n");
		ensure_equals(tracker->sentContent[1], "101\n");
		ensure_equals(tracker->sentContent[2], "102\n");
		ensure_equals(tracker->sentContent[3], "1000\n");
		ensure_equals(listDir("tmp.clouddata").size(), 0u);
	}

	TEST_METHOD(9) {
		set_test_name("It stops sending recorded usage points upon encountering a server error");

		createFile("tmp.clouddata/100", "100\n");
		createFile("tmp.clouddata/101", "101\n");
		createFile("tmp.clouddata/102", "102\n");
		init();
		setLogLevel(LVL_ERROR);
		tracker->curlCodes.push_back(CURLE_OK);
		tracker->curlCodes.push_back(CURLE_OK);
		tracker->curlCodes.push_back(CURLE_COULDNT_CONNECT); // "102" cannot be sent
		tracker->runOneCycle();
		ensure_equals(tracker->sentCount(), 3u);
		ensure_equals(tracker->sentContent[0], "100\n");
		ensure_equals(tracker->sentContent[1], "101\n");
		ensure_equals(tracker->sentContent[2], "102\n");
		vector<string> files = listDir("tmp.clouddata");
		ensure_equals(files.size(), 2u);
		ensure_equals(files[0], "1000");
		ensure_equals(files[1], "102");
	}

	TEST_METHOD(10) {
		set_test_name("It aborts if, after sending recorded usage points, too many usage points still remain");

		createFile("tmp.clouddata/100", "100\n");
		createFile("tmp.clouddata/101", "101\n");
		createFile("tmp.clouddata/102", "102\n");
		createFile("tmp.clouddata/103", "103\n");
		init();
		setLogLevel(LVL_ERROR);
		tracker->threshold = 3;
		tracker->curlCodes.push_back(CURLE_OK);
		tracker->curlCodes.push_back(CURLE_COULDNT_CONNECT);
		try {
			tracker->runOneCycle();
			fail("AbortException expected");
		} catch (const AbortException &e) {
			ensure(string(e.what()).find("for more than 3 days") != string::npos);
		}
		ensure_equals(tracker->sentCount(), 2u);
		ensure_equals(tracker->sentContent[0], "100\n");
		ensure_equals(tracker->sentContent[1], "101\n");
		vector<string> files = listDir("tmp.clouddata");
		ensure_equals("4 data points should be left", files.size(), 4u);
		ensure_equals(files[0], "1000");
		ensure_equals(files[1], "101");
		ensure_equals(files[2], "102");
		ensure_equals(files[3], "103");
	}

	TEST_METHOD(11) {
		set_test_name("Leaves the recorded usage point in the data directory and does "
			"not send data points to the server if autoSend is false");

		init("tmp.clouddata", false);
		tracker->responseDataList.push_back("{\"status\": \"error\", \"message\": \"lp0 on fire\"}");
		tracker->runOneCycle();
		vector<string> files = listDir("tmp.clouddata");
		ensure_equals(files.size(), 1u);
		ensure_equals(files[0], "1000");
	}
}
