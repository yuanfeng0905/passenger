/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_ADMIN_CONTROLLER_H_
#define _PASSENGER_ADMIN_CONTROLLER_H_


#include <agents/LoggingAgent/LoggingServer.h>
#include <MessageServer.h>
#include <sstream>

namespace Passenger {

using namespace std;
using namespace boost;
using namespace oxt;


class AdminController: public MessageServer::Handler {
private:
	struct SpecificContext: public MessageServer::ClientContext {
	};

	typedef MessageServer::CommonClientContext CommonClientContext;

	LoggingServerPtr server;


	/*********************************************
	 * Message handler methods
	 *********************************************/

	void processStatus(CommonClientContext &commonContext, SpecificContext *specificContext,
		const vector<string> &args)
	{
		TRACE_POINT();
		commonContext.passSecurity();
		stringstream stream;
		server->dump(stream);
		writeScalarMessage(commonContext.fd, stream.str());
	}

public:
	AdminController(const LoggingServerPtr &server) {
		this->server = server;
	}

	virtual MessageServer::ClientContextPtr newClient(CommonClientContext &commonContext) {
		return boost::make_shared<SpecificContext>();
	}

	virtual bool processMessage(CommonClientContext &commonContext,
	                            MessageServer::ClientContextPtr &_specificContext,
	                            const vector<string> &args)
	{
		SpecificContext *specificContext = (SpecificContext *) _specificContext.get();
		try {
			if (isCommand(args, "status", 0)) {
				processStatus(commonContext, specificContext, args);
			} else {
				return false;
			}
		} catch (const SecurityException &) {
			/* Client does not have enough rights to perform a certain action.
			 * It has already been notified of this; ignore exception and move on.
			 */
		}
		return true;
	}
};


} // namespace Passenger

#endif /* _PASSENGER_ADMIN_CONTROLLER_H_ */
