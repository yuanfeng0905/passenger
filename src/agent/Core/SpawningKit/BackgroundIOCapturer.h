/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_SPAWNING_KIT_BACKGROUND_IO_CAPTURER_H_
#define _PASSENGER_SPAWNING_KIT_BACKGROUND_IO_CAPTURER_H_

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <oxt/backtrace.hpp>
#include <oxt/system_calls.hpp>
#include <string>
#include <vector>
#include <cstring>

#include <sys/types.h>

#include <Logging.h>
#include <FileDescriptor.h>
#include <StaticString.h>
#include <Utils.h>
#include <Utils/StrIntUtils.h>

namespace Passenger {
namespace SpawningKit {

using namespace std;


/**
 * Given a file descriptor, captures its output in a background thread
 * and also forwards it immediately to a target file descriptor.
 * Call stop() to stop the background thread and to obtain the captured
 * output so far.
 */
class BackgroundIOCapturer {
private:
	FileDescriptor fd;
	pid_t pid;
	const char *channelName;
	boost::mutex dataSyncher;
	string data;
	oxt::thread *thr;

	void capture() {
		TRACE_POINT();
		while (!this_thread::interruption_requested()) {
			char buf[1024 * 8];
			ssize_t ret;

			UPDATE_TRACE_POINT();
			ret = syscalls::read(fd, buf, sizeof(buf));
			int e = errno;
			this_thread::disable_syscall_interruption dsi;
			if (ret == 0) {
				break;
			} else if (ret == -1) {
				if (e != EAGAIN && e != EWOULDBLOCK) {
					P_WARN("Background I/O capturer error: " <<
						strerror(e) << " (errno=" << e << ")");
					break;
				}
			} else {
				{
					boost::lock_guard<boost::mutex> l(dataSyncher);
					data.append(buf, ret);
				}
				UPDATE_TRACE_POINT();
				if (ret == 1 && buf[0] == '\n') {
					printAppOutput(pid, channelName, "", 0);
				} else {
					vector<StaticString> lines;
					if (ret > 0 && buf[ret - 1] == '\n') {
						ret--;
					}
					split(StaticString(buf, ret), '\n', lines);
					foreach (const StaticString line, lines) {
						printAppOutput(pid, channelName, line.data(), line.size());
					}
				}
			}
		}
	}

public:
	BackgroundIOCapturer(const FileDescriptor &_fd, pid_t _pid, const char *_channelName)
		: fd(_fd),
		  pid(_pid),
		  channelName(_channelName),
		  thr(NULL)
		{ }

	~BackgroundIOCapturer() {
		TRACE_POINT();
		if (thr != NULL) {
			this_thread::disable_interruption di;
			this_thread::disable_syscall_interruption dsi;
			thr->interrupt_and_join();
			delete thr;
			thr = NULL;
		}
	}

	const FileDescriptor &getFd() const {
		return fd;
	}

	void start() {
		assert(thr == NULL);
		thr = new oxt::thread(boost::bind(&BackgroundIOCapturer::capture, this),
			"Background I/O capturer", 64 * 1024);
	}

	string stop() {
		TRACE_POINT();
		assert(thr != NULL);
		this_thread::disable_interruption di;
		this_thread::disable_syscall_interruption dsi;
		thr->interrupt_and_join();
		delete thr;
		thr = NULL;
		boost::lock_guard<boost::mutex> l(dataSyncher);
		return data;
	}

	void appendToBuffer(const StaticString &dataToAdd) {
		TRACE_POINT();
		boost::lock_guard<boost::mutex> l(dataSyncher);
		data.append(dataToAdd.data(), dataToAdd.size());
	}
};

typedef boost::shared_ptr<BackgroundIOCapturer> BackgroundIOCapturerPtr;


} // namespace SpawningKit
} // namespace Passenger

#endif /* _PASSENGER_SPAWNING_KIT_BACKGROUND_IO_CAPTURER_H_ */
