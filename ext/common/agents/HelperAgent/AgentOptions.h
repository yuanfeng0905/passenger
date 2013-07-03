/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_HELPER_AGENT_OPTIONS_H_
#define _PASSENGER_HELPER_AGENT_OPTIONS_H_

#include <sys/types.h>
#include <string>
#include <Utils.h>
#include <Utils/VariantMap.h>

namespace Passenger {

using namespace std;


struct AgentOptions {
	pid_t   webServerPid;
	string  serverInstanceDir;
	string  tempDir;
	bool    userSwitching;
	string  defaultUser;
	string  defaultGroup;
	string  passengerRoot;
	string  defaultRubyCommand;
	unsigned int generationNumber;
	unsigned int maxPoolSize;
	unsigned int maxInstancesPerApp;
	unsigned int poolIdleTime;
	string requestSocketFilename;
	string requestSocketPassword;
	string adminSocketAddress;
	string exitPassword;
	string loggingAgentAddress;
	string loggingAgentPassword;
	string adminToolStatusPassword;
	vector<string> prestartUrls;

	string requestSocketLink;
	string cloudLicensingDataDir;
	string cloudLicensingBaseUrl;
	string cloudLicensingCert;
	string cloudLicensingProxy;

	AgentOptions() { }

	AgentOptions(const VariantMap &options) {
		// Required options for which a default is already set by the Watchdog.
		passengerRoot      = options.get("passenger_root");
		tempDir            = options.get("temp_dir");
		userSwitching      = options.getBool("user_switching");
		defaultRubyCommand = options.get("default_ruby");
		defaultUser        = options.get("default_user");
		defaultGroup       = options.get("default_group");
		maxPoolSize        = options.getInt("max_pool_size");
		maxInstancesPerApp = options.getInt("max_instances_per_app");
		poolIdleTime       = options.getInt("pool_idle_time");

		// Required options only set by the Watchdog.
		webServerPid          = options.getPid("web_server_pid");
		serverInstanceDir     = options.get("server_instance_dir");
		generationNumber      = options.getInt("generation_number");
		requestSocketFilename = options.get("request_socket_filename");
		requestSocketPassword = options.get("request_socket_password");
		if (requestSocketPassword == "-") {
			requestSocketPassword = "";
		}
		adminSocketAddress    = options.get("helper_agent_admin_socket_address");
		exitPassword          = options.get("helper_agent_exit_password");
		loggingAgentAddress   = options.get("logging_agent_address");
		loggingAgentPassword  = options.get("logging_agent_password");
		adminToolStatusPassword = options.get("admin_tool_status_password");
		
		// Optional options.
		prestartUrls          = options.getStrSet("prestart_urls", false);
		requestSocketLink     = options.get("request_socket_link", false);
		cloudLicensingDataDir = options.get("cloud_licensing_data_dir", false,
			getHomeDir() + "/.passenger-enterprise/cloud_usage_data");
		cloudLicensingCert    = options.get("cloud_licensing_cert", false);
		cloudLicensingBaseUrl = options.get("cloud_licensing_base_url", false);
		cloudLicensingProxy   = options.get("cloud_licensing_proxy", false);
	}
};


} // namespace Passenger

#endif /* _PASSENGER_HELPER_AGENT_OPTIONS_H_ */
