/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014-2015 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */
#include <Core/Controller.h>

/*************************************************************************
 *
 * Miscellaneous functions for Core::Controller
 *
 *************************************************************************/

namespace Passenger {
namespace Core {

using namespace std;
using namespace boost;


/****************************
 *
 * Public methods
 *
 ****************************/


void
Controller::disconnectLongRunningConnections(const StaticString &gupid) {
	vector<Client *> clients;
	vector<Client *>::iterator v_it, v_end;
	Client *client;

	// We collect all clients in a vector so that we don't have to worry about
	// `activeClients` being mutated while we work.
	TAILQ_FOREACH (client, &activeClients, nextClient.activeOrDisconnectedClient) {
		P_ASSERT_EQ(client->getConnState(), Client::ACTIVE);
		if (client->currentRequest != NULL) {
			Request *req = client->currentRequest;
			if (req->httpState >= Request::COMPLETE
			 && req->upgraded()
			 && req->options.abortWebsocketsOnProcessShutdown
			 && req->session != NULL
			 && req->session->getGupid() == gupid)
			{
				if (getLogLevel() >= LVL_INFO) {
					char clientName[32];
					unsigned int size;
					const LString *host;
					StaticString hostStr;

					size = getClientName(client, clientName, sizeof(clientName));
					if (req->host != NULL && req->host->size > 0) {
						host = psg_lstr_make_contiguous(req->host, req->pool);
						hostStr = StaticString(host->start->data, host->size);
					}
					P_INFO("[" << getServerName() << "] Disconnecting client " <<
						StaticString(clientName, size) << ": " <<
						hostStr << StaticString(req->path.start->data, req->path.size));
				}
				refClient(client, __FILE__, __LINE__);
				clients.push_back(client);
			}
		}
	}

	// Disconnect each eligible client.
	v_end = clients.end();
	for (v_it = clients.begin(); v_it != v_end; v_it++) {
		client = *v_it;
		Client *c = client;
		disconnect(&client);
		unrefClient(c, __FILE__, __LINE__);
	}
}

Controller::BenchmarkMode
Controller::parseBenchmarkMode(const StaticString mode) {
	if (mode.empty()) {
		return BM_NONE;
	} else if (mode == "after_accept") {
		return BM_AFTER_ACCEPT;
	} else if (mode == "before_checkout") {
		return BM_BEFORE_CHECKOUT;
	} else if (mode == "after_checkout") {
		return BM_AFTER_CHECKOUT;
	} else if (mode == "response_begin") {
		return BM_RESPONSE_BEGIN;
	} else {
		return BM_UNKNOWN;
	}
}


} // namespace Core
} // namespace Passenger
