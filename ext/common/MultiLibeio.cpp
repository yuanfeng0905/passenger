/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#include <MultiLibeio.h>
#include <oxt/thread.hpp>
#include <oxt/system_calls.hpp>
#include <cassert>

#ifndef PREAD_AND_PWRITE_ARE_NOT_THREADSAFE
	#ifdef __APPLE__
		#define PREAD_AND_PWRITE_ARE_NOT_THREADSAFE 1
	#else
		#define PREAD_AND_PWRITE_ARE_NOT_THREADSAFE 0
	#endif
#endif


namespace Passenger {

using namespace oxt;

static boost::mutex syncher;
static condition_variable cond;
static bool shouldPoll = false;
static oxt::thread *thr = NULL;
static bool quit = false;


struct Data {
	SafeLibevPtr libev;
	MultiLibeio::Callback callback;

	Data(const SafeLibevPtr &_libev, const MultiLibeio::Callback &_callback)
		: libev(_libev),
		  callback(_callback)
	{
		assert(_libev != NULL);
	}
};

struct CustomData: public Data {
	MultiLibeio::ExecuteCallback execute;

	CustomData(const SafeLibevPtr &_libev,
		const MultiLibeio::Callback &_callback,
		const MultiLibeio::ExecuteCallback &_execute)
		: Data(_libev, _callback),
		  execute(_execute)
		{ }
};


static void
threadMain() {
	unique_lock<boost::mutex> l(syncher);
	while (!quit) {
		while (!shouldPoll && !quit) {
			cond.wait(l);
		}
		if (!quit) {
			shouldPoll = false;
			l.unlock();
			eio_poll();
			l.lock();
		}
	}
}

static void
wantPoll() {
	boost::lock_guard<boost::mutex> l(syncher);
	shouldPoll = true;
	cond.notify_one();
}

static int
dispatch(eio_req *req) {
	auto_ptr<Data> data((Data *) req->data);
	assert(data->libev != NULL);
	data->libev->runLater(boost::bind(data->callback, *req));
	return 0;
}

static void
executeWrapper(eio_req *req) {
	CustomData *data = (CustomData *) req->data;
	data->execute(req);
}

#if PREAD_AND_PWRITE_ARE_NOT_THREADSAFE
	static boost::mutex preadWriteLock;

	static void
	lockedPread(int fd, void *buf, size_t length, off_t offset, eio_req *req) {
		boost::lock_guard<boost::mutex> l(preadWriteLock);
		req->result = pread(fd, buf, length, offset);
	}

	static void
	lockedPwrite(int fd, void *buf, size_t length, off_t offset, eio_req *req) {
		boost::lock_guard<boost::mutex> l(preadWriteLock);
		req->result = pwrite(fd, buf, length, offset);
	}
#endif

void
MultiLibeio::init() {
	eio_init(wantPoll, NULL);
	thr = new oxt::thread(threadMain, "MultiLibeio dispatcher", 1024 * 64);
}

void
MultiLibeio::shutdown() {
	unique_lock<boost::mutex> l(syncher);
	quit = true;
	cond.notify_one();
	l.unlock();
	thr->join();
	delete thr;
	thr = NULL;
	quit = false;
}

#define MAKE_REQUEST(code) \
	eio_req *result; \
	Data *data = new Data(libev, callback); \
	code \
	if (result == NULL) { \
		delete data; \
		return NULL; \
	} else { \
		return result; \
	}

eio_req *
MultiLibeio::open(const char *path, int flags, mode_t mode, int pri, const Callback &callback) {
	MAKE_REQUEST(
		result = eio_open(path, flags, mode, pri, dispatch, data);
	);
}

eio_req *
MultiLibeio::read(int fd, void *buf, size_t length, off_t offset, int pri, const Callback &callback) {
	#if PREAD_AND_PWRITE_ARE_NOT_THREADSAFE
		return custom(boost::bind(lockedPread, fd, buf, length, offset, _1),
			pri, callback);
	#else
		MAKE_REQUEST(
			result = eio_read(fd, buf, length, offset, pri, dispatch, data);
		);
	#endif
}

eio_req *
MultiLibeio::write(int fd, void *buf, size_t length, off_t offset, int pri, const Callback &callback) {
	#if PREAD_AND_PWRITE_ARE_NOT_THREADSAFE
		return custom(boost::bind(lockedPwrite, fd, buf, length, offset, _1),
			pri, callback);
	#else
		MAKE_REQUEST(
			result = eio_write(fd, buf, length, offset, pri, dispatch, data);
		);
	#endif
}

eio_req *
MultiLibeio::custom(const ExecuteCallback &execute, int pri, const Callback &callback) {
	CustomData *data = new CustomData(libev, callback, execute);
	eio_req *result = eio_custom(executeWrapper, pri, dispatch, data);
	if (result == NULL) {
		delete data;
		return NULL;
	} else {
		return result;
	}
}


} // namespace Passenger
