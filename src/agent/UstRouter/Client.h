/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2015 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_UST_ROUTER_CLIENT_H_
#define _PASSENGER_UST_ROUTER_CLIENT_H_

#include <set>
#include <string>
#include <UstRouter/Transaction.h>
#include <ServerKit/Server.h>
#include <MessageReadersWriters.h>

namespace Passenger {
namespace UstRouter {

using namespace std;
using namespace boost;


class Client: public ServerKit::BaseClient {
public:
	enum State {
		READING_AUTH_USERNAME,
		READING_AUTH_PASSWORD,
		READING_MESSAGE,
		READING_MESSAGE_BODY
	};

	enum Type {
		UNINITIALIZED,
		LOGGER
	};

	ArrayMessage arrayReader;
	ScalarMessage scalarReader;

	State state;
	Type type;
	string nodeName;

	/**
	 * Set of transaction IDs opened by this client.
	 * @invariant This is a subset of the transaction IDs in the 'transactions' member.
	 */
	set<string> openTransactions;

	struct {
		TransactionPtr transaction;
		string timestamp;
		bool ack;
	} logCommandParams;

	Client(void *server)
		: ServerKit::BaseClient(server)
		{ }

	const char *getStateName() const {
		switch (state) {
		case READING_AUTH_USERNAME:
			return "UNINITIALIZED";
		case READING_AUTH_PASSWORD:
			return "READING_AUTH_PASSWORD";
		case READING_MESSAGE:
			return "READING_MESSAGE";
		case READING_MESSAGE_BODY:
			return "READING_MESSAGE_BODY";
		default:
			return "UNKNOWN";
		}
	}

	const char *getTypeName() const {
		switch (type) {
		case UNINITIALIZED:
			return "UNINITIALIZED";
		case LOGGER:
			return "LOGGER";
		default:
			return "UNKNOWN";
		}
	}

	DEFINE_SERVER_KIT_BASE_CLIENT_FOOTER(Client);
};


} // namespace UstRouter
} // namespace Passenger

#endif /* _PASSENGER_UST_ROUTER_TRANSACTION_H_ */
