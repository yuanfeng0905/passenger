/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_APPLICATION_POOL2_CONTEXT_H_
#define _PASSENGER_APPLICATION_POOL2_CONTEXT_H_

#include <boost/thread.hpp>
#include <boost/pool/object_pool.hpp>
#include <Exceptions.h>
#include <SpawningKit/Factory.h>
#include <Utils/ClassUtils.h>

namespace Passenger {
namespace ApplicationPool2 {


using namespace boost;

class Session;
class Process;


/**
 * State shared by Pool, Group, Process and Session. It contains statistics
 * and counters, memory management objects, configuration objects, etc.
 * This struct was introduced so that Group, Process and Sessions don't have
 * to depend on Pool (which introduces circular dependencies).
 *
 * The fields are separated in several groups. Each group may have its own mutex.
 * If it does, then all operations on any of the fields in that group requires
 * grabbing the mutex unless documented otherwise.
 */
class Context {
private:
	/****** Memory management objects *****/

	P_RO_PROPERTY_REF(private, boost::mutex, MmSyncher);
	P_RO_PROPERTY_REF(private, object_pool<Session>, SessionObjectPool);
	P_RO_PROPERTY_REF(private, object_pool<Process>, ProcessObjectPool);


	/****** Configuration objects ******/

	P_PROPERTY_CONST_REF(private, SpawningKit::FactoryPtr, SpawningKitFactory);


public:
	/****** Initialization ******/

	Context()
		: mSessionObjectPool(64, 1024),
		  mProcessObjectPool(4, 64)
		{ }

	void finalize() {
		if (mSpawningKitFactory == NULL) {
			throw RuntimeException("spawningKitFactory must be set");
		}
	}


	/****** Configuration objects ******/

	const SpawningKit::ConfigPtr &getSpawningKitConfig() const {
		return mSpawningKitFactory->getConfig();
	}
};


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL2_CONTEXT_H_ */
