/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2014 Phusion
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
		execl(agentFilename.c_str(), "PassengerAgent", "logger",
			// Some extra space to allow the child process to change its process title.
			"                                                ", (char *) 0);
	}

	virtual void sendStartupArguments(pid_t pid, FileDescriptor &fd) {
		VariantMap options = *agentsOptions;
		options.erase("server_password");
		options.erase("server_authorizations");
		options.writeToFd(fd);
	}

	virtual bool processStartupInfo(pid_t pid, FileDescriptor &fd, const vector<string> &args) {
		return args[0] == "initialized";
	}

public:
	LoggingAgentWatcher(const WorkingObjectsPtr &wo)
		: AgentWatcher(wo)
	{
		agentFilename = wo->resourceLocator->getAgentsDir() + "/PassengerAgent";
	}

	virtual void reportAgentsInformation(VariantMap &report) {
		const VariantMap &options = *agentsOptions;
		report.set("logging_agent_address", options.get("logging_agent_address"));
		report.set("logging_agent_password", options.get("logging_agent_password"));
	}
};
