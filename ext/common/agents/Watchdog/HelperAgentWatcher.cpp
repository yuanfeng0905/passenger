/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2014 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

class HelperAgentWatcher: public AgentWatcher {
protected:
	string agentFilename;

	virtual const char *name() const {
		return "Phusion Passenger helper agent";
	}

	virtual string getExeFilename() const {
		return agentFilename;
	}

	virtual void execProgram() const {
		if (hasEnvOption("PASSENGER_RUN_HELPER_AGENT_IN_VALGRIND", false)) {
			execlp("valgrind", "valgrind", "--dsymutil=yes",
				agentFilename.c_str(), "PassengerAgent", "server",
				// Some extra space to allow the child process to change its process title.
				"                                                ", (char *) 0);
		} else {
			execl(agentFilename.c_str(), "PassengerAgent", "server",
				// Some extra space to allow the child process to change its process title.
				"                                                ", (char *) 0);
		}
	}

	virtual void sendStartupArguments(pid_t pid, FileDescriptor &fd) {
		VariantMap options = *agentsOptions;
		options.erase("logging_agent_authorizations");
		options.writeToFd(fd);
	}

	virtual bool processStartupInfo(pid_t pid, FileDescriptor &fd, const vector<string> &args) {
		return args[0] == "initialized";
	}

public:
	HelperAgentWatcher(const WorkingObjectsPtr &wo)
		: AgentWatcher(wo)
	{
		agentFilename = wo->resourceLocator->getAgentsDir() + "/PassengerAgent";
	}

	virtual void reportAgentsInformation(VariantMap &report) {
		const VariantMap &options = *agentsOptions;
		vector<string> addresses = options.getStrSet("server_addresses");
		report.set("server_address", addresses.front());
		report.set("server_password", options.get("server_password"));
	}
};
