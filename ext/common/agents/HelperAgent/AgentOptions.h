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
#include <boost/shared_ptr.hpp>
#include <Utils.h>
#include <Utils/VariantMap.h>

namespace Passenger {

using namespace std;
using namespace boost;


struct AgentOptions: public VariantMap {
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
	unsigned int poolIdleTime;
	string requestSocketFilename;
	string requestSocketPassword;
	string adminSocketAddress;
	string exitPassword;
	string loggingAgentAddress;
	string loggingAgentPassword;
	string adminToolStatusPassword;
	vector<string> prestartUrls;

	bool testBinary;
	string requestSocketLink;
	string licensingBaseUrl;
	string licensingProxy;
	bool licensingServerCheckCert;
	bool licensingDataPointsAutoSend;

	AgentOptions() { }

	AgentOptions(const VariantMap &options)
		: VariantMap(options)
	{
		testBinary = options.get("test_binary", false) == "1";
		if (testBinary) {
			return;
		}

		// Required options for which a default is already set by the Watchdog.
		passengerRoot      = options.get("passenger_root");
		tempDir            = options.get("temp_dir");
		userSwitching      = options.getBool("user_switching");
		defaultRubyCommand = options.get("default_ruby");
		defaultUser        = options.get("default_user");
		defaultGroup       = options.get("default_group");
		maxPoolSize        = options.getInt("max_pool_size");
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
		licensingBaseUrl = options.get("licensing_base_url", false);
		licensingProxy   = options.get("licensing_proxy", false);
		licensingServerCheckCert = options.getBool("licensing_server_check_cert",
			false, true);
		licensingDataPointsAutoSend = options.getBool(
			"licensing_data_points_auto_send", false, true);
	}

	// We don't initialize the value in the constructor because the
	// home dir changes after calling lowerPrivilege().
	string getLicensingDataDir() const {
		return get("licensing_data_dir", false,
			getHomeDir() + "/.passenger-enterprise/usage_data");
	}
};

typedef boost::shared_ptr<AgentOptions> AgentOptionsPtr;


} // namespace Passenger

#endif /* _PASSENGER_HELPER_AGENT_OPTIONS_H_ */
