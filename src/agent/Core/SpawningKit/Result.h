/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_SPAWNING_KIT_RESULT_H_
#define _PASSENGER_SPAWNING_KIT_RESULT_H_

#include <FileDescriptor.h>
#include <jsoncpp/json.h>

namespace Passenger {
namespace SpawningKit {


/**
 * Represents the result of a spawning operation. It is a JSON document
 * containing information about the spawned process, such as its PID,
 * GUPID, etc. In addition, it contains two file descriptors.
 */
struct Result: public Json::Value {
	FileDescriptor adminSocket;
	FileDescriptor errorPipe;
};


} // namespace SpawningKit
} // namespace Passenger

#endif /* _PASSENGER_SPAWNING_KIT_RESULT_H_ */
