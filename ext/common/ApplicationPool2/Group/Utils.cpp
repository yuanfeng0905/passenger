/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

// This file is included inside the Group class.

private:

static void
runAllActions(const boost::container::vector<Callback> &actions) {
	boost::container::vector<Callback>::const_iterator it, end = actions.end();
	for (it = actions.begin(); it != end; it++) {
		(*it)();
	}
}

static void
doCleanupSpawner(SpawningKit::SpawnerPtr spawner) {
	spawner->cleanup();
}

unsigned int
generateStickySessionId() {
	unsigned int result;

	while (true) {
		result = (unsigned int) rand();
		if (result != 0 && findProcessWithStickySessionId(result) == NULL) {
			return result;
		}
	}
	// Never reached; shut up compiler warning.
	return 0;
}

/**
 * Persists options into this Group. Called at creation time and at restart time.
 * Values will be persisted into `destination`. Or if it's NULL, into `this->options`.
 */
void
resetOptions(const Options &newOptions, Options *destination = NULL) {
	if (destination == NULL) {
		destination = &this->options;
	}
	*destination = newOptions;
	destination->persist(newOptions);
	destination->clearPerRequestFields();
	destination->apiKey    = getApiKey().toStaticString();
	destination->groupUuid = uuid;
}

/**
 * Merges some of the new options from the latest get() request into this Group.
 */
void
mergeOptions(const Options &other) {
	options.maxRequests      = other.maxRequests;
	options.minProcesses     = other.minProcesses;
	options.statThrottleRate = other.statThrottleRate;
	options.maxPreloaderIdleTime = other.maxPreloaderIdleTime;
	options.maxProcesses     = other.maxProcesses;
}
