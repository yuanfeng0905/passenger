/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

class LoggingAgentWatcher: public AgentWatcher {
protected:
	string agentFilename;
	string socketAddress;
	
	virtual const char *name() const {
		return "Phusion Passenger logging agent";
	}
	
	virtual string getExeFilename() const {
		return agentFilename;
	}
	
	virtual void execProgram() const {
		execl(agentFilename.c_str(), "PassengerLoggingAgent", (char *) 0);
	}
	
	virtual void sendStartupArguments(pid_t pid, FileDescriptor &fd) {
		VariantMap options = agentsOptions;
		options.set("logging_agent_address", loggingAgentAddress);
		options.set("logging_agent_password", loggingAgentPassword);
		options.set("logging_agent_admin_address", loggingAgentAdminAddress);
		options.writeToFd(fd);
	}
	
	virtual bool processStartupInfo(pid_t pid, FileDescriptor &fd, const vector<string> &args) {
		if (args[0] == "initialized") {
			return true;
		} else {
			return false;
		}
	}
	
public:
	LoggingAgentWatcher(const ResourceLocator &resourceLocator) {
		agentFilename = resourceLocator.getAgentsDir() + "/PassengerLoggingAgent";
	}
	
	virtual void reportAgentsInformation(VariantMap &report) {
		report
			.set("logging_socket_address", loggingAgentAddress)
			.set("logging_socket_password", loggingAgentPassword)
			.set("logging_socket_admin_address", loggingAgentAdminAddress);
	}
};
