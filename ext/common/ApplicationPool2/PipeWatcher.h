/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2012-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_APPLICATION_POOL_PIPE_WATCHER_H_
#define _PASSENGER_APPLICATION_POOL_PIPE_WATCHER_H_

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <sys/types.h>
#include <FileDescriptor.h>

namespace Passenger {
namespace ApplicationPool2 {

using namespace boost;


/** A PipeWatcher lives until the file descriptor is closed. */
struct PipeWatcher: public boost::enable_shared_from_this<PipeWatcher> {
	// For unit tests.
	typedef boost::function<void (const char *data, unsigned int size)> DataCallback;
	static DataCallback onData;

	FileDescriptor fd;
	const char *name;
	pid_t pid;
	bool started;
	boost::mutex startSyncher;
	boost::condition_variable startCond;

	PipeWatcher(const FileDescriptor &_fd, const char *name, pid_t pid);
	void initialize();
	void start();
	static void threadMain(boost::shared_ptr<PipeWatcher> self);
	void threadMain();
};

typedef boost::shared_ptr<PipeWatcher> PipeWatcherPtr;


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL_PIPE_WATCHER_H_ */
