/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_FILE_HANDLE_GUARD_H_
#define _PASSENGER_FILE_HANDLE_GUARD_H_

#include <boost/noncopyable.hpp>
#include <oxt/system_calls.hpp>
#include <cstdio>

namespace Passenger {

using namespace oxt;


/**
 * Simple scope-level guard object for ensuring that a given FILE handle
 * or file descriptor is closed when the object goes out of scope.
 * Unlike FileDescriptor, which internally uses a shared_ptr and allocates
 * data structure on the heap, this class is much more lightweight and
 * does not allocate anything on the heap.
 */
class FileHandleGuard: public boost::noncopyable {
private:
	enum { T_FILE, T_FD } type;
	union {
		FILE *file;
		int fd;
	} u;
public:
	FileHandleGuard(FILE *file) {
		type = T_FILE;
		u.file = file;
	}
	
	FileHandleGuard(int fd) {
		type = T_FD;
		u.fd = fd;
	}
	
	void close() {
		if (type == T_FILE && u.file != NULL) {
			syscalls::fclose(u.file);
			u.file = NULL;
		} else if (type == T_FD && u.fd != -1) {
			syscalls::close(u.fd);
			u.fd = -1;
		}
	}
	
	~FileHandleGuard() {
		this_thread::disable_syscall_interruption dsi;
		close();
	}
};


} // namespace Passenger

#endif /* _PASSENGER_FILE_HANDLE_GUARD_H_ */
