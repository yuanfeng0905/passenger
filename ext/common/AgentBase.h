/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
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

bool feedbackFdAvailable();
VariantMap initializeAgent(int argc, char *argv[], const char *processName);

}

#endif /* _PASSENGER_AGENT_BASE_H_ */
