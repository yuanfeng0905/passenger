/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011, 2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _BACKGROUND_EVENT_LOOP_H_
#define _BACKGROUND_EVENT_LOOP_H_

#include <boost/shared_ptr.hpp>
#include <string>

extern "C" {
	struct ev_loop;
	struct ev_async;
}

namespace Passenger {
	using namespace std;
	using namespace boost;

	class SafeLibev;
	struct BackgroundEventLoopPrivate;

	/**
	 * Implements a libev event loop that runs in a background thread.
	 */
	struct BackgroundEventLoop {
		struct ev_loop *loop;
		ev_async *async;
		boost::shared_ptr<SafeLibev> safe;
		BackgroundEventLoopPrivate *priv;

		BackgroundEventLoop(bool scalable = false, bool useLibeio = false);
		~BackgroundEventLoop();

		void start(const string &threadName = "", unsigned int stackSize = 1024 * 1024);
		void stop();
		bool isStarted() const;
	};

}

#endif /* _BACKGROUND_EVENT_LOOP_H_ */
