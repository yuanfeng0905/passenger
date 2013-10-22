/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_MULTI_LIBEIO_H_
#define _PASSENGER_MULTI_LIBEIO_H_

#include <boost/function.hpp>
#include <eio.h>
#include <SafeLibev.h>

namespace Passenger {

using namespace boost;


class MultiLibeio {
private:
	SafeLibevPtr libev;

public:
	typedef boost::function<void (eio_req *req)> ExecuteCallback;
	typedef boost::function<void (eio_req req)> Callback;

	static void init();
	static void shutdown();

	MultiLibeio() { }

	MultiLibeio(const SafeLibevPtr &_libev)
		: libev(_libev)
		{ }

	const SafeLibevPtr &getLibev() const {
		return libev;
	}

	eio_req *open(const char *path, int flags, mode_t mode, int pri, const Callback &callback);
	eio_req *read(int fd, void *buf, size_t length, off_t offset, int pri, const Callback &callback);
	eio_req *write(int fd, void *buf, size_t length, off_t offset, int pri, const Callback &callback);
	eio_req *custom(const ExecuteCallback &execute, int pri, const Callback &callback);
};


} // namespace Passenger

#endif /* _PASSENGER_MULTI_LIBEIO_H_ */
