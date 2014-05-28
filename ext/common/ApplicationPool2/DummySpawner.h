/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_APPLICATION_POOL2_DUMMY_SPAWNER_H_
#define _PASSENGER_APPLICATION_POOL2_DUMMY_SPAWNER_H_

#include <ApplicationPool2/Spawner.h>

namespace Passenger {
namespace ApplicationPool2 {

using namespace std;
using namespace boost;
using namespace oxt;


class DummySpawner: public Spawner {
private:
	SpawnerConfigPtr config;
	boost::mutex lock;
	unsigned int count;
	
public:
	unsigned int cleanCount;
	
	DummySpawner(const ResourceLocator &resourceLocator, const SpawnerConfigPtr &_config)
		: Spawner(resourceLocator),
		  config(_config)
	{
		count = 0;
		cleanCount = 0;
	}
	
	virtual ProcessPtr spawn(const Options &options) {
		TRACE_POINT();
		possiblyRaiseInternalError(options);

		SocketPair adminSocket = createUnixSocketPair();
		SocketListPtr sockets = boost::make_shared<SocketList>();
		sockets->add("main", "tcp://127.0.0.1:1234", "session", config->concurrency);
		syscalls::usleep(config->spawnTime);
		
		boost::lock_guard<boost::mutex> l(lock);
		count++;
		ProcessPtr process = boost::make_shared<Process>(
			(pid_t) count, "gupid-" + toString(count),
			toString(count),
			adminSocket.second, FileDescriptor(), sockets,
			SystemTime::getUsec(), SystemTime::getUsec());
		process->dummy = true;
		return process;
	}

	virtual bool cleanable() const {
		return true;
	}

	virtual void cleanup() {
		cleanCount++;
	}
};

typedef boost::shared_ptr<DummySpawner> DummySpawnerPtr;


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL2_DUMMY_SPAWNER_H_ */
