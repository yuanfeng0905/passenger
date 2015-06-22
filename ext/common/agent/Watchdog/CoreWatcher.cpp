/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

class CoreWatcher: public AgentWatcher {
protected:
	string agentFilename;

	virtual const char *name() const {
		return SHORT_PROGRAM_NAME " core";
	}

	virtual string getExeFilename() const {
		return agentFilename;
	}

	virtual void execProgram() const {
		if (hasEnvOption("PASSENGER_RUN_CORE_IN_VALGRIND", false)) {
			execlp("valgrind", "valgrind", "--dsymutil=yes", "--track-origins=yes", "--leak-check=full",
				agentFilename.c_str(), "core",
				// Some extra space to allow the child process to change its process title.
				"                                                ", (char *) 0);
		} else {
			execl(agentFilename.c_str(), AGENT_EXE, "core",
				// Some extra space to allow the child process to change its process title.
				"                                                ", (char *) 0);
		}
	}

	virtual void sendStartupArguments(pid_t pid, FileDescriptor &fd) {
		VariantMap options = *agentsOptions;
		options.erase("ust_router_authorizations");
		options.writeToFd(fd);
	}

	virtual bool processStartupInfo(pid_t pid, FileDescriptor &fd, const vector<string> &args) {
		return args[0] == "initialized";
	}

public:
	CoreWatcher(const WorkingObjectsPtr &wo)
		: AgentWatcher(wo)
	{
		agentFilename = wo->resourceLocator->findSupportBinary(AGENT_EXE);
	}

	virtual void reportAgentsInformation(VariantMap &report) {
		const VariantMap &options = *agentsOptions;
		vector<string> addresses = options.getStrSet("core_addresses");
		report.set("core_address", addresses.front());
		report.set("core_password", options.get("core_password"));

		// For backwards compatibility:
		report.set("server_address", addresses.front());
		report.set("server_password", options.get("core_password"));
	}
};
