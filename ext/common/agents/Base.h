/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010, 2011, 2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_AGENT_BASE_H_
#define _PASSENGER_AGENT_BASE_H_

/** Common code for all agents. */

#include "Utils/VariantMap.h"

namespace Passenger {

typedef void (*DiagnosticsDumper)(void *userData);

bool feedbackFdAvailable();
VariantMap initializeAgent(int argc, char *argv[], const char *processName);
void installAbortHandler();
void installDiagnosticsDumper(DiagnosticsDumper func, void *userData);

}

#endif /* _PASSENGER_AGENT_BASE_H_ */
