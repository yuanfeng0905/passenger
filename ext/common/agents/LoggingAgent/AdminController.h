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
	
	const LoggingServer *server;
	
	
	/*********************************************
	 * Message handler methods
	 *********************************************/
	
	void processStatus(CommonClientContext &commonContext, SpecificContext *specificContext, const vector<string> &args) {
		TRACE_POINT();
		stringstream stream;
		server->dump(stream);
		writeScalarMessage(commonContext.fd, stream.str());
	}
	
	bool isCommand(const vector<string> &args, const string &command, unsigned int nargs = 0) const {
		return args.size() == nargs + 1 && args[0] == command;
	}
	
public:
	AdminController(const LoggingServer *server) {
		this->server = server;
	}
	
	virtual MessageServer::ClientContextPtr newClient(CommonClientContext &commonContext) {
		return make_shared<SpecificContext>();
	}
	
	virtual bool processMessage(CommonClientContext &commonContext,
	                            MessageServer::ClientContextPtr &_specificContext,
	                            const vector<string> &args)
	{
		SpecificContext *specificContext = (SpecificContext *) _specificContext.get();
		if (isCommand(args, "status", 0)) {
			processStatus(commonContext, specificContext, args);
		} else {
			return false;
		}
		return true;
	}
};


} // namespace Passenger

#endif /* _PASSENGER_ADMIN_CONTROLLER_H_ */
