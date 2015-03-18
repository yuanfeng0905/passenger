/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_SPAWNING_KIT_DUMMY_SPAWNER_H_
#define _PASSENGER_SPAWNING_KIT_DUMMY_SPAWNER_H_

#include <SpawningKit/Spawner.h>

namespace Passenger {
namespace SpawningKit {

using namespace std;
using namespace boost;
using namespace oxt;


class DummySpawner: public Spawner {
private:
	boost::mutex lock;
	unsigned int count;

public:
	unsigned int cleanCount;

	DummySpawner(const ConfigPtr &_config)
		: Spawner(_config)
	{
		count = 0;
		cleanCount = 0;
	}

	virtual Result spawn(const Options &options) {
		TRACE_POINT();
		possiblyRaiseInternalError(options);

		SocketPair adminSocket = createUnixSocketPair();
		Result::Socket socket;
		socket.name = "main";
		socket.address = "tcp://127.0.0.1:1234";
		socket.protocol = "session";
		socket.concurrency = config->concurrency;

		syscalls::usleep(config->spawnTime);

		boost::lock_guard<boost::mutex> l(lock);
		Result result;

		count++;
		result.type = Result::DUMMY_PROCESS;
		result.pid = count;
		result.setGupid("gupid-" + toString(count));
		result.adminSocket = adminSocket.second;
		result.sockets.push_back(socket);
		result.spawnerCreationTime = SystemTime::getUsec();
		result.spawnStartTime = SystemTime::getUsec();
		return result;
	}

	virtual bool cleanable() const {
		return true;
	}

	virtual void cleanup() {
		cleanCount++;
	}
};

typedef boost::shared_ptr<DummySpawner> DummySpawnerPtr;


} // namespace SpawningKit
} // namespace Passenger

#endif /* _PASSENGER_SPAWNING_KIT_DUMMY_SPAWNER_H_ */
