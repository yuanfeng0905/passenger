/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_REQUEST_HANDLER_CLIENT_H_
#define _PASSENGER_REQUEST_HANDLER_CLIENT_H_

#include <ev++.h>
#include <ostream>
#include <ServerKit/HttpClient.h>
#include <Core/RequestHandler/Request.h>

namespace Passenger {

using namespace std;
using namespace boost;
using namespace ApplicationPool2;


class Client: public ServerKit::BaseHttpClient<Request> {
public:
	ev_tstamp connectedAt;

	Client(void *server)
		: ServerKit::BaseHttpClient<Request>(server)
	{
		SERVER_KIT_BASE_HTTP_CLIENT_INIT();
	}

	DEFINE_SERVER_KIT_BASE_HTTP_CLIENT_FOOTER(Client, Request);
};


} // namespace Passenger

#endif /* _PASSENGER_REQUEST_HANDLER_CLIENT_H_ */
