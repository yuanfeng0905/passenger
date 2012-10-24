/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_BACKTRACES_SERVER_H_
#define _PASSENGER_BACKTRACES_SERVER_H_

#include <oxt/thread.hpp>
#include <oxt/backtrace.hpp>
#include <MessageServer.h>
#include <Utils/MessageIO.h>

namespace Passenger {

using namespace std;

/**
 * A MessageServer handler for reporting the backtraces of all running oxt::threads.
 */
class BacktracesServer: public MessageServer::Handler {
public:
	virtual bool processMessage(MessageServer::CommonClientContext &commonContext,
	                            MessageServer::ClientContextPtr &handlerSpecificContext,
	                            const vector<string> &args)
	{
		TRACE_POINT();
		if (args[0] == "backtraces") {
			UPDATE_TRACE_POINT();
			commonContext.requireRights(Account::INSPECT_BACKTRACES);
			writeScalarMessage(commonContext.fd, oxt::thread::all_backtraces());
			return true;
		} else {
			return false;
		}
	}
};

} // namespace Passenger

#endif /* _PASSENGER_BACKTRACES_SERVER_H_ */
