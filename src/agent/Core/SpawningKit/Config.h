/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_SPAWNING_KIT_CONFIG_H_
#define _PASSENGER_SPAWNING_KIT_CONFIG_H_

#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <string>
#include <cstddef>

#include <ResourceLocator.h>
#include <RandomGenerator.h>
#include <Exceptions.h>
#include <Utils/VariantMap.h>
#include <Core/UnionStation/Core.h>

namespace Passenger {
namespace ApplicationPool2 {
	class Options;
} // namespace ApplicationPool2
} // namespace Passenger

namespace Passenger {
namespace SpawningKit {

using namespace std;


struct Config;
typedef ApplicationPool2::Options Options;
typedef boost::shared_ptr<Config> ConfigPtr;

typedef void (*ErrorHandler)(const ConfigPtr &config, SpawnException &e, const Options &options);
typedef boost::function<void (const char *data, unsigned int size)> OutputHandler;

struct Config {
	// Used by error pages and hooks.
	ResourceLocator *resourceLocator;
	const VariantMap *agentsOptions;
	ErrorHandler errorHandler;

	// Used for Union Station logging.
	UnionStation::CorePtr unionStationCore;

	// Used by SmartSpawner and DirectSpawner.
	RandomGeneratorPtr randomGenerator;
	string instanceDir;

	// Used by DummySpawner and SpawnerFactory.
	unsigned int concurrency;
	unsigned int spawnerCreationSleepTime;
	unsigned int spawnTime;

	// Used by PipeWatcher.
	OutputHandler outputHandler;

	// Other.
	void *data;

	Config()
		: resourceLocator(NULL),
		  agentsOptions(NULL),
		  errorHandler(NULL),
		  concurrency(1),
		  spawnerCreationSleepTime(0),
		  spawnTime(0),
		  data(NULL)
		{ }

	void finalize() {
		TRACE_POINT();
		if (resourceLocator == NULL) {
			throw RuntimeException("ResourceLocator not initialized");
		}
		if (randomGenerator == NULL) {
			randomGenerator = boost::make_shared<RandomGenerator>();
		}
	}
};


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_SPAWNING_KIT_CONFIG_H_ */
