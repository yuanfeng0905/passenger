/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_APPLICATION_POOL_SESSION_H_
#define _PASSENGER_APPLICATION_POOL_SESSION_H_

#include <sys/types.h>
#include <boost/shared_ptr.hpp>
#include <oxt/macros.hpp>
#include <oxt/system_calls.hpp>
#include <oxt/backtrace.hpp>
#include <ApplicationPool2/Common.h>
#include <ApplicationPool2/Socket.h>
#include <FileDescriptor.h>
#include <Utils/ScopeGuard.h>

namespace Passenger {
namespace ApplicationPool2 {

using namespace oxt;


/**
 * Represents a communication session with a process. A communication session
 * within Phusion Passenger is usually a single request + response but the API
 * allows arbitrary I/O. See Process's class overview for normal usage of Session.
 *
 * Not thread-safe, but Pool's and Process's API encourage that
 * a Session is only used by 1 thread and then thrown away.
 */
class Session {
public:
	typedef void (*Callback)(Session *session);

private:
	ProcessPtr process;
	/** Socket to use for this session. Guaranteed to be alive thanks to the 'process' reference. */
	Socket *socket;

	Connection connection;
	FileDescriptor theFd;
	bool closed;

	void deinitiate(bool success) {
		connection.fail = !success;
		socket->checkinConnection(connection);
		connection.fd = -1;
		theFd = FileDescriptor();
	}

	void callOnInitiateFailure() {
		if (OXT_LIKELY(onInitiateFailure != NULL)) {
			onInitiateFailure(this);
		}
	}

	void callOnClose() {
		if (OXT_LIKELY(onClose != NULL)) {
			onClose(this);
		}
		closed = true;
	}

public:
	Callback onInitiateFailure;
	Callback onClose;

	Session(const ProcessPtr &_process, Socket *_socket)
		: process(_process),
		  socket(_socket),
		  closed(false),
		  onInitiateFailure(NULL),
		  onClose(NULL)
		{ }

	~Session() {
		TRACE_POINT();
		// If user doesn't close() explicitly, we penalize performance.
		if (OXT_LIKELY(initiated())) {
			deinitiate(false);
		}
		if (OXT_LIKELY(!closed)) {
			callOnClose();
		}
	}

	const string &getConnectPassword() const;
	pid_t getPid() const;
	const string &getGupid() const;
	unsigned int getStickySessionId() const;
	const GroupPtr getGroup() const;
	void requestOOBW();
	int kill(int signo);

	bool isClosed() const {
		return closed;
	}

	/**
	 * @pre !isClosed()
	 * @post result != NULL
	 */
	const ProcessPtr &getProcess() const {
		assert(!closed);
		return process;
	}

	Socket *getSocket() const {
		return socket;
	}

	const string &getProtocol() const {
		return socket->protocol;
	}

	void initiate() {
		assert(!closed);
		ScopeGuard g(boost::bind(&Session::callOnInitiateFailure, this));
		connection = socket->checkoutConnection();
		connection.fail = true;
		theFd = FileDescriptor(connection.fd, false);
		g.clear();
	}

	bool initiated() const {
		return connection.fd != -1;
	}

	const FileDescriptor &fd() const {
		return theFd;
	}

	/**
	 * This Session object becomes fully unsable after closing.
	 */
	void close(bool success) {
		if (OXT_LIKELY(initiated())) {
			deinitiate(success);
		}
		if (OXT_LIKELY(!closed)) {
			callOnClose();
		}
	}
};

typedef boost::shared_ptr<Session> SessionPtr;


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL2_SESSION_H_ */
