/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_AGENT_BASE_H_
#define _PASSENGER_AGENT_BASE_H_

/** Common initialization code for all agents. */

#include <cstddef>
#include <Utils/VariantMap.h>

namespace Passenger {

typedef void (*DiagnosticsDumper)(void *userData);
typedef void (*OptionParserFunc)(int argc, const char **argv, VariantMap &options);
typedef void (*PreinitializationFunc)(VariantMap &options);

const char *getEnvString(const char *name, const char *defaultValue = NULL);
bool hasEnvOption(const char *name, bool defaultValue = false);

bool feedbackFdAvailable();
VariantMap initializeAgent(int argc, char **argv[], const char *processName,
	OptionParserFunc optionParser = NULL, PreinitializationFunc preinit = NULL,
	int argStartIndex = 1);
void initializeAgentOptions(VariantMap &options, PreinitializationFunc preinit = NULL);
void installAgentAbortHandler();
void installDiagnosticsDumper(DiagnosticsDumper func, void *userData);

}

#endif /* _PASSENGER_AGENT_BASE_H_ */
