/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2016 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */
#include <oxt/thread.hpp>
#include <set>
#include <cerrno>
#include <cstring>
#include <string.h>
#include <WatchdogLauncher.h>
#include <Exceptions.h>

using namespace std;
using namespace boost;
using namespace oxt;


PsgVariantMap *
psg_variant_map_new() {
	return (PsgVariantMap *) new Passenger::VariantMap();
}

char *
psg_variant_map_get_optional(PsgVariantMap *m,
	const char *name)
{
	Passenger::VariantMap *vm = (Passenger::VariantMap *) m;
	string result = vm->get(name, false, "");
	if (result.empty()) {
		return (char *) NULL;
	}
	return strdup(result.c_str());
}

void
psg_variant_map_set(PsgVariantMap *m,
	const char *name,
	const char *value,
	unsigned int value_len)
{
	Passenger::VariantMap *vm = (Passenger::VariantMap *) m;
	vm->set(name, string(value, value_len));
}

void
psg_variant_map_set2(PsgVariantMap *m,
	const char *name,
	unsigned int name_len,
	const char *value,
	unsigned int value_len)
{
	Passenger::VariantMap *vm = (Passenger::VariantMap *) m;
	vm->set(string(name, name_len), string(value, value_len));
}

void
psg_variant_map_set_int(PsgVariantMap *m,
	const char *name,
	int value)
{
	Passenger::VariantMap *vm = (Passenger::VariantMap *) m;
	vm->setInt(name, value);
}

void
psg_variant_map_set_bool(PsgVariantMap *m,
	const char *name,
	int value)
{
	Passenger::VariantMap *vm = (Passenger::VariantMap *) m;
	vm->setBool(name, value);
}

void
psg_variant_map_set_strset(PsgVariantMap *m,
	const char *name,
	const char **strs,
	unsigned int count)
{
	Passenger::VariantMap *vm = (Passenger::VariantMap *) m;
	std::set<string> the_set;

	for (unsigned int i = 0; i < count; i++) {
		the_set.insert(strs[i]);
	}
	vm->setStrSet(name, the_set);
}

void
psg_variant_map_free(PsgVariantMap *m) {
	delete (Passenger::VariantMap *) m;
}


PsgWatchdogLauncher *
psg_watchdog_launcher_new(PsgIntegrationMode mode, char **error_message) {
	return (PsgWatchdogLauncher *) new Passenger::WatchdogLauncher(mode);
}

int
psg_watchdog_launcher_start(PsgWatchdogLauncher *l,
	const char *passengerRoot,
	PsgVariantMap *extraParams,
	const PsgAfterForkCallback afterFork,
	void *callbackArgument,
	char **errorMessage)
{
	Passenger::WatchdogLauncher *launcher = (Passenger::WatchdogLauncher *) l;
	this_thread::disable_syscall_interruption dsi;
	try {
		boost::function<void ()> afterForkFunctionObject;

		if (afterFork != NULL) {
			afterForkFunctionObject = boost::bind(afterFork, callbackArgument, extraParams);
		}
		launcher->start(passengerRoot,
			*((Passenger::VariantMap *) extraParams),
			afterForkFunctionObject);
		return 1;
	} catch (const Passenger::SystemException &e) {
		errno = e.code();
		*errorMessage = strdup(e.what());
		return 0;
	} catch (const std::exception &e) {
		errno = -1;
		*errorMessage = strdup(e.what());
		return 0;
	}
}

const char *
psg_watchdog_launcher_get_core_address(PsgWatchdogLauncher *l, unsigned int *size) {
	Passenger::WatchdogLauncher *launcher = (Passenger::WatchdogLauncher *) l;
	if (size != NULL) {
		*size = launcher->getCoreAddress().size();
	}
	return launcher->getCoreAddress().c_str();
}

const char *
psg_watchdog_launcher_get_core_password(PsgWatchdogLauncher *l, unsigned int *size) {
	Passenger::WatchdogLauncher *launcher = (Passenger::WatchdogLauncher *) l;
	if (size != NULL) {
		*size = launcher->getCorePassword().size();
	}
	return launcher->getCorePassword().c_str();
}

const char *
psg_watchdog_launcher_get_instance_dir(PsgWatchdogLauncher *l, unsigned int *size) {
	Passenger::WatchdogLauncher *launcher = (Passenger::WatchdogLauncher *) l;
	if (size != NULL) {
		*size = launcher->getInstanceDir().size();
	}
	return launcher->getInstanceDir().c_str();
}

pid_t
psg_watchdog_launcher_get_pid(PsgWatchdogLauncher *l) {
	Passenger::WatchdogLauncher *launcher = (Passenger::WatchdogLauncher *) l;
	return launcher->getPid();
}

void
psg_watchdog_launcher_detach(PsgWatchdogLauncher *l) {
	Passenger::WatchdogLauncher *launcher = (Passenger::WatchdogLauncher *) l;
	launcher->detach();
}

void
psg_watchdog_launcher_free(PsgWatchdogLauncher *l) {
	Passenger::WatchdogLauncher *launcher = (Passenger::WatchdogLauncher *) l;
	delete launcher;
}
