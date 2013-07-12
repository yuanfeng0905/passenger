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
		options.set("logging_agent_address", wo->loggingAgentAddress);
		options.set("logging_agent_password", wo->loggingAgentPassword);
		options.set("logging_agent_admin_address", wo->loggingAgentAdminAddress);
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
	LoggingAgentWatcher(const WorkingObjectsPtr &wo)
		: AgentWatcher(wo)
	{
		agentFilename = wo->resourceLocator->getAgentsDir() + "/PassengerLoggingAgent";
	}
	
	virtual void reportAgentsInformation(VariantMap &report) {
		report
			.set("logging_socket_address", wo->loggingAgentAddress)
			.set("logging_socket_password", wo->loggingAgentPassword)
			.set("logging_socket_admin_address", wo->loggingAgentAdminAddress);
	}
};
