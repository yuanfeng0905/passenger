/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_SPAWNING_KIT_FACTORY_H_
#define _PASSENGER_SPAWNING_KIT_FACTORY_H_

#include <SpawningKit/Spawner.h>
#include <SpawningKit/SmartSpawner.h>
#include <SpawningKit/DirectSpawner.h>
#include <SpawningKit/DummySpawner.h>

namespace Passenger {
namespace SpawningKit {

using namespace std;
using namespace boost;
using namespace oxt;


class Factory {
private:
	boost::mutex syncher;
	ConfigPtr config;
	DummySpawnerPtr dummySpawner;

	SpawnerPtr tryCreateSmartSpawner(const Options &options) {
		string dir = config->resourceLocator->getHelperScriptsDir();
		vector<string> preloaderCommand;
		if (options.appType == "rack") {
			preloaderCommand.push_back(options.ruby);
			preloaderCommand.push_back(dir + "/rack-preloader.rb");
		} else {
			return SpawnerPtr();
		}
		return boost::make_shared<SmartSpawner>(preloaderCommand,
			options, config);
	}

public:
	Factory(const ConfigPtr &_config)
		: config(_config)
		{ }

	virtual ~Factory() { }

	virtual SpawnerPtr create(const Options &options) {
		if (options.spawnMethod == "smart" || options.spawnMethod == "smart-lv2") {
			SpawnerPtr spawner = tryCreateSmartSpawner(options);
			if (spawner == NULL) {
				spawner = boost::make_shared<DirectSpawner>(config);
			}
			return spawner;
		} else if (options.spawnMethod == "direct" || options.spawnMethod == "conservative") {
			boost::shared_ptr<DirectSpawner> spawner = boost::make_shared<DirectSpawner>(
				config);
			return spawner;
		} else if (options.spawnMethod == "dummy") {
			syscalls::usleep(config->spawnerCreationSleepTime);
			return getDummySpawner();
		} else {
			throw ArgumentException("Unknown spawn method '" + options.spawnMethod + "'");
		}
	}

	/**
	 * SpawnerFactory always returns the same DummyFactory object upon
	 * creating a dummy spawner. This allows unit tests to easily
	 * set debugging options on the spawner.
	 */
	DummySpawnerPtr getDummySpawner() {
		boost::lock_guard<boost::mutex> l(syncher);
		if (dummySpawner == NULL) {
			dummySpawner = boost::make_shared<DummySpawner>(config);
		}
		return dummySpawner;
	}

	/**
	 * All created Spawner objects share the same Config object.
	 */
	const ConfigPtr &getConfig() const {
		return config;
	}
};

typedef boost::shared_ptr<Factory> FactoryPtr;


} // namespace SpawningKit
} // namespace Passenger

#endif /* _PASSENGER_SPAWNING_KIT_FACTORY_H_ */
