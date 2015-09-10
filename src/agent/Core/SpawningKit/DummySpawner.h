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

#include <Core/SpawningKit/Spawner.h>
#include <boost/atomic.hpp>

namespace Passenger {
namespace SpawningKit {

using namespace std;
using namespace boost;
using namespace oxt;


class DummySpawner: public Spawner {
private:
	boost::atomic<unsigned int> count;

public:
	unsigned int cleanCount;

	DummySpawner(const ConfigPtr &_config)
		: Spawner(_config),
		  count(1),
		  cleanCount(0)
		{ }

	virtual Result spawn(const Options &options) {
		TRACE_POINT();
		possiblyRaiseInternalError(options);

		syscalls::usleep(config->spawnTime);

		SocketPair adminSocket = createUnixSocketPair(__FILE__, __LINE__);
		unsigned int number = count.fetch_add(1, boost::memory_order_relaxed);
		Result result;
		Json::Value socket;

		socket["name"] = "main";
		socket["address"] = "tcp://127.0.0.1:1234";
		socket["protocol"] = "session";
		socket["concurrency"] = config->concurrency;

		result["type"] = "dummy";
		result["pid"] = number;
		result["gupid"] = "gupid-" + toString(number);
		result["spawner_creation_time"] = (Json::UInt64) SystemTime::getUsec();
		result["spawn_start_time"] = (Json::UInt64) SystemTime::getUsec();
		result["sockets"].append(socket);
		result.adminSocket = adminSocket.second;

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
