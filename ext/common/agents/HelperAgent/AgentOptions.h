/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011, 2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_HELPER_AGENT_OPTIONS_H_
#define _PASSENGER_HELPER_AGENT_OPTIONS_H_

#include <sys/types.h>
#include <string>
#include <Utils/VariantMap.h>
#include <Utils/Base64.h>

namespace Passenger {

using namespace std;


struct AgentOptions {
	pid_t   webServerPid;
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
	string requestSocketPassword;
	string messageSocketPassword;
	string loggingAgentAddress;
	string loggingAgentPassword;
	string prestartUrls;

	string requestSocketLink;

	AgentOptions() { }

	AgentOptions(const VariantMap &options) {
		// Required options for which a default is already set by the Watchdog.
		passengerRoot = options.get("passenger_root");
		tempDir               = options.get("temp_dir");
		userSwitching = options.getBool("user_switching");
		defaultRubyCommand   = options.get("default_ruby");
		defaultUser   = options.get("default_user");
		defaultGroup  = options.get("default_group");
		maxPoolSize        = options.getInt("max_pool_size");
		maxInstancesPerApp = options.getInt("max_instances_per_app");
		poolIdleTime       = options.getInt("pool_idle_time");

		// Required options only set by the Watchdog.
		webServerPid          = options.getPid("web_server_pid");
		generationNumber      = options.getInt("generation_number");
		requestSocketPassword = Base64::decode(options.get("request_socket_password"));
		messageSocketPassword = Base64::decode(options.get("message_socket_password"));
		loggingAgentAddress   = options.get("logging_agent_address");
		loggingAgentPassword  = options.get("logging_agent_password");
		
		// Optional options.
		prestartUrls          = options.get("prestart_urls", false, "");
		requestSocketLink     = options.get("request_socket_link", false);
	}
};


} // namespace Passenger

#endif /* _PASSENGER_HELPER_AGENT_OPTIONS_H_ */
