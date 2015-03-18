/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

// This file is included inside the Pool class.

private:

ProcessPtr findOldestIdleProcess(const Group *exclude = NULL) const {
	ProcessPtr oldestIdleProcess;

	GroupMap::ConstIterator g_it(groups);
	while (*g_it != NULL) {
		const GroupPtr &group = g_it.getValue();
		if (group.get() == exclude || group->hasSpawnError) {
			g_it.next();
			continue;
		}
		const ProcessList &processes = group->enabledProcesses;
		ProcessList::const_iterator p_it, p_end = processes.end();
		for (p_it = processes.begin(); p_it != p_end; p_it++) {
			const ProcessPtr process = *p_it;
			if (process->busyness() == 0
			     && (oldestIdleProcess == NULL
			         || process->lastUsed < oldestIdleProcess->lastUsed)
			) {
				oldestIdleProcess = process;
			}
		}
		g_it.next();
	}

	return oldestIdleProcess;
}

ProcessPtr findBestProcessToTrash() const {
	ProcessPtr oldestProcess;

	GroupMap::ConstIterator g_it(groups);
	while (*g_it != NULL) {
		const GroupPtr &group = g_it.getValue();
		const ProcessList &processes = group->enabledProcesses;
		ProcessList::const_iterator p_it, p_end = processes.end();
		for (p_it = processes.begin(); p_it != p_end; p_it++) {
			const ProcessPtr process = *p_it;
			if (oldestProcess == NULL
			 || process->lastUsed < oldestProcess->lastUsed) {
				oldestProcess = process;
			}
		}
		g_it.next();
	}

	return oldestProcess;
}
