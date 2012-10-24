/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_APPLICATION_POOL_PIPE_WATCHER_H_
#define _PASSENGER_APPLICATION_POOL_PIPE_WATCHER_H_

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <SafeLibev.h>
#include <FileDescriptor.h>
#include <ev++.h>

namespace Passenger {
namespace ApplicationPool2 {

using namespace boost;


struct PipeWatcher: public enable_shared_from_this<PipeWatcher> {
	SafeLibevPtr libev;
	FileDescriptor fd;
	ev::io watcher;
	shared_ptr<PipeWatcher> selfPointer;
	int fdToForwardTo;

	PipeWatcher(const SafeLibevPtr &_libev,
		const FileDescriptor &_fd,
		int _fdToForwardTo);
	~PipeWatcher();
	void start();
	void onReadable(ev::io &io, int revents);
};

typedef shared_ptr<PipeWatcher> PipeWatcherPtr;


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL_PIPE_WATCHER_H_ */
