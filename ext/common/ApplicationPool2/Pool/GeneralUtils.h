/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

// This file is included inside the Pool class.

protected:

static void runAllActions(const boost::container::vector<Callback> &actions) {
	boost::container::vector<Callback>::const_iterator it, end = actions.end();
	for (it = actions.begin(); it != end; it++) {
		(*it)();
	}
}

static void runAllActionsWithCopy(boost::container::vector<Callback> actions) {
	runAllActions(actions);
}

void verifyInvariants() const {
	// !a || b: logical equivalent of a IMPLIES b.
	#ifndef NDEBUG
	if (!selfchecking) {
		return;
	}
	assert(!( !getWaitlist.empty() ) || ( atFullCapacityUnlocked() ));
	assert(!( !atFullCapacityUnlocked() ) || ( getWaitlist.empty() ));
	#endif
}

void verifyExpensiveInvariants() const {
	#ifndef NDEBUG
	if (!selfchecking) {
		return;
	}
	vector<GetWaiter>::const_iterator it, end = getWaitlist.end();
	for (it = getWaitlist.begin(); it != end; it++) {
		const GetWaiter &waiter = *it;
		const SuperGroupPtr *superGroup;
		assert(!superGroups.lookup(waiter.options.getAppGroupName(), &superGroup));
	}
	#endif
}

void fullVerifyInvariants() const {
	TRACE_POINT();
	verifyInvariants();
	UPDATE_TRACE_POINT();
	verifyExpensiveInvariants();
	UPDATE_TRACE_POINT();

	SuperGroupMap::ConstIterator sg_it(superGroups);
	while (*sg_it != NULL) {
		const SuperGroupPtr &superGroup = sg_it.getValue();
		superGroup->verifyInvariants();
		foreach (GroupPtr group, superGroup->groups) {
			group->verifyInvariants();
			group->verifyExpensiveInvariants();
		}
		sg_it.next();
	}
}

bool runHookScripts(const char *name,
	const boost::function<void (HookScriptOptions &)> &setup) const
{
	if (agentsOptions != NULL) {
		string hookName = string("hook_") + name;
		string spec = agentsOptions->get(hookName, false);
		if (!spec.empty()) {
			HookScriptOptions options;
			options.agentsOptions = agentsOptions;
			options.name = name;
			options.spec = spec;
			setup(options);
			return Passenger::runHookScripts(options);
		} else {
			return true;
		}
	} else {
		return true;
	}
}

static const char *maybePluralize(unsigned int count, const char *singular, const char *plural) {
	if (count == 1) {
		return singular;
	} else {
		return plural;
	}
}


public:

const SpawnerConfigPtr &getSpawnerConfig() const {
	return spawnerFactory->getConfig();
}

const UnionStation::CorePtr &getUnionStationCore() const {
	return getSpawnerConfig()->unionStationCore;
}

const RandomGeneratorPtr &getRandomGenerator() const {
	return getSpawnerConfig()->randomGenerator;
}
