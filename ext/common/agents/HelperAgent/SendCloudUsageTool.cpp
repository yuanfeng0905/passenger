/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

#include <curl/curl.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <oxt/initialize.hpp>
#include <oxt/system_calls.hpp>
#include <Logging.h>
#include <Constants.h>
#include <Exceptions.h>
#include <CloudUsageTracker.h>
#include <ResourceLocator.h>
#include <Utils.h>
#include <Utils/License.h>

using namespace std;
using namespace Passenger;

namespace {
	struct Options {
		string passengerRoot;
		string datadir;
		string baseUrl;
		string certificate;
		bool sslCertCheck;
		bool help;

		Options()
			: sslCertCheck(true),
			  help(false)
			{ }
	};
}

static void usage();


static bool
isFlag(const char *arg, char shortFlagName, const char *longFlagName) {
	return strcmp(arg, longFlagName) == 0
		|| (shortFlagName != '\0' && arg[0] == '-'
			&& arg[1] == shortFlagName && arg[2] == '\0');
}

static bool
isValueFlag(int argc, int i, const char *arg, char shortFlagName, const char *longFlagName) {
	if (isFlag(arg, shortFlagName, longFlagName)) {
		if (argc >= i + 2) {
			return true;
		} else {
			fprintf(stderr, "ERROR: extra argument required for %s\n", arg);
			usage();
			exit(1);
			return false; // Never reached
		}
	} else {
		return false;
	}
}

static void
usage() {
	printf("Usage: passenger-config send-cloud-usage [OPTIONS]\n");
	printf("Send cloud usage points to the Phusion Passenger licensing server.\n");
	printf("\n");
	printf("Options:\n");
	printf("      --passenger-root PATH   The location to the " PROGRAM_NAME " source\n");
	printf("                              directory\n");
	printf("      --no-ssl-cert-check     Do not verify SSL certificate\n");
	printf("      --base-url              Base URL to contact. Default:\n");
	printf("                              https://www.phusionpassenger.com\n");
	printf("  -h, --help                  Show this help\n");
}

static Options
parseOptions(int argc, char *argv[]) {
	Options options;
	int i = 2;

	while (i < argc) {
		if (isValueFlag(argc, i, argv[i], '\0', "--passenger-root")) {
			options.passengerRoot = argv[i + 1];
			i += 2;
		} else if (isFlag(argv[i], '\0', "--no-ssl-cert-check")) {
			options.sslCertCheck = false;
			i++;
		} else if (isValueFlag(argc, i, argv[i], '\0', "--base-url")) {
			options.baseUrl = argv[i + 1];
			i += 2;
		} else if (isFlag(argv[i], 'h', "--help")) {
			options.help = true;
			i++;
		} else {
			fprintf(stderr, "ERROR: unrecognized argument %s\n", argv[i]);
			usage();
			exit(1);
		}
	}
	return options;
}

static void
initialize(Options &options) {
	if (options.help) {
		usage();
		exit(0);
	} else if (options.passengerRoot.empty()) {
		fprintf(stderr, "ERROR: please set --passenger-root.\n");
		usage();
		exit(1);
	}

	setLogLevel(LVL_DEBUG);

	passenger_enterprise_license_init();
	char *error = passenger_enterprise_license_check();
	if (error != NULL) {
		string message = error;
		free(error);
		throw RuntimeException(message);
	}

	CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
	if (code != CURLE_OK) {
		P_CRITICAL("Could not initialize libcurl: " << curl_easy_strerror(code));
		exit(1);
	}

	oxt::initialize();
	setup_syscall_interruption_support();

	ResourceLocator resourceLocator(options.passengerRoot);
	options.datadir = getHomeDir() + "/.passenger-enterprise/usage_data";
	if (options.sslCertCheck) {
		options.certificate = resourceLocator.getResourcesDir() + "/licensing_server.crt";
	}
}

static void
licenseErrorHandler(const string &message) {
	P_CRITICAL(message);
	exit(1);
}

int
sendCloudUsageMain(int argc, char *argv[]) {
	Options options = parseOptions(argc, argv);
	initialize(options);

	CloudUsageTracker tracker(options.datadir, options.baseUrl, options.certificate);
	tracker.licenseErrorHandler = licenseErrorHandler;
	if (tracker.runOneCycle()) {
		return 0;
	} else {
		return 1;
	}
}
