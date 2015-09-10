/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

class UstRouterWatcher: public AgentWatcher {
protected:
	string agentFilename;
	string socketAddress;

	virtual const char *name() const {
		return SHORT_PROGRAM_NAME " UstRouter";
	}

	virtual string getExeFilename() const {
		return agentFilename;
	}

	virtual void execProgram() const {
		execl(agentFilename.c_str(), AGENT_EXE, "ust-router",
			// Some extra space to allow the child process to change its process title.
			"                                                ", (char *) 0);
	}

	virtual void sendStartupArguments(pid_t pid, FileDescriptor &fd) {
		VariantMap options = *agentsOptions;
		options.erase("core_password");
		options.erase("core_authorizations");
		options.writeToFd(fd);
	}

	virtual bool processStartupInfo(pid_t pid, FileDescriptor &fd, const vector<string> &args) {
		return args[0] == "initialized";
	}

public:
	UstRouterWatcher(const WorkingObjectsPtr &wo)
		: AgentWatcher(wo)
	{
		agentFilename = wo->resourceLocator->findSupportBinary(AGENT_EXE);
	}

	virtual void reportAgentsInformation(VariantMap &report) {
		const VariantMap &options = *agentsOptions;
		report.set("ust_router_address", options.get("ust_router_address"));
		report.set("ust_router_password", options.get("ust_router_password"));

		// For backward compatibilty:
		report.set("logging_agent_address", options.get("ust_router_address"));
		report.set("logging_agent_password", options.get("ust_router_password"));
	}
};
