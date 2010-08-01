/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_CHANGE_NOTIFIER_H_
#define _PASSENGER_CHANGE_NOTIFIER_H_

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>
#include <set>
#include <ev++.h>
#include "DataStoreId.h"
#include "../EventedClient.h"
#include "../FileDescriptor.h"
#include "../StaticString.h"
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cmath>
#include "DataStoreId.h"
#include "../EventedClient.h"
#include "../FileDescriptor.h"
#include "../StaticString.h"
#include "../Logging.h"
#include "../MessageReadersWriters.h"
#include "../Utils/StrIntUtils.h"
#include "../Utils/ScopeGuard.h"

namespace Passenger {

using namespace std;
using namespace boost;


class ChangeNotifier {
public:
	typedef function<string (const StaticString &groupName, const StaticString &nodeName,
		const StaticString &category)> GetLastPosFunction;
	
private:
	struct DataStore;
	typedef shared_ptr<DataStore> DataStorePtr;
	typedef map<DataStoreId, DataStorePtr> DataStoreMap;
	
	class Client: public EventedClient {
	public:
		Client(struct ev_loop *loop, const FileDescriptor &fd)
			: EventedClient(loop, fd)
			{ }
		
		ChangeNotifier *notifier;
		ArrayMessage messageReader;
		DataStoreMap processing;
	};
	
	struct DataStore {
		static const int MAX_RETRIES = 10;
		
		Client *client;
		DataStoreId identifier;
		
		string lastPos;
		int lastPosCheckRetry;
		ev::timer lastPosCheckRetryTimer;
		
		void retryLastPositionCheck(ev::timer &timer, int revents) {
			client->notifier->retryLastPositionCheck(client, this);
		}
	};
	
	typedef set<Client *> ClientSet;
	
	struct ev_loop *loop;
	ClientSet clients;
	
	
	static void destroyClient(Client *client) {
		delete client;
	}
	
	static void onReadable(EventedClient *client) {
		ChangeNotifier *self = (ChangeNotifier *) client->userData;
		char buf[1024 * 8];
		ssize_t ret;
		
		client->ref();
		ScopeGuard guard(boost::bind(&Client::unref, client));
		
		ret = read(client->fd, buf, sizeof(buf));
		if (ret == -1) {
			if (errno != EAGAIN) {
				int e = errno;
				self->logSystemError(client, "Cannot read() from client", e);
				client->disconnect();
			}
		} else if (ret == 0) {
			client->disconnect();
		} else {
			self->processData((Client *) client, buf, ret);
		}
	}
	
	static void onDisconnect(EventedClient *client) {
		ChangeNotifier *self = (ChangeNotifier *) client->userData;
		self->clients.erase((Client *) client);
		client->unref();
	}
	
	void processData(Client *client, const char *data, size_t size) {
		size_t consumed = 0;
		ArrayMessage &messageReader = client->messageReader;
		
		while (consumed < size && client->ioAllowed()) {
			consumed += messageReader.feed(data, size);
			if (messageReader.done()) {
				if (messageReader.hasError() || messageReader.value().empty()) {
					logError(client, "Client sent an invalid message");
					client->disconnect();
				} else {
					processMessage(client, messageReader.value());
					messageReader.reset();
				}
			}
		}
	}
	
	void processMessage(Client *client, const vector<StaticString> &args) {
		if (args[0] == "done") {
			processDoneMessage(client, args);
		} else {
			logError(client, "Client sent an unknown message");
			client->disconnect();
		}
	}
	
	string getClientName(const EventedClient *client) const {
		return toString(client);
	}
	
	void logError(const EventedClient *client, const string &message) {
		P_ERROR("Error in client " << getClientName(client) << ": " << message);
	}
	
	void logSystemError(const EventedClient *client, const string &message, int errorCode) {
		P_ERROR("Error in client " << getClientName(client) << ": " <<
			message << ": " << strerror(errorCode) << " (" << errorCode << ")");
	}
	
	string createHumanReadableIdentifier(const DataStoreId &id) const {
		return "[" + id.getGroupName() + ", " +	id.getNodeName() +
			", " + id.getCategory() + "]";
	}
	
	bool clientIsProcessing(const Client *client, const DataStoreId &id) const {
		return client->processing.find(id) != client->processing.end();
	}
	
	void markClientProcessing(Client *client, const DataStoreId &id) {
		DataStorePtr store(new DataStore());
		store->client = client;
		store->identifier = id;
		store->lastPosCheckRetry = 0;
		store->lastPosCheckRetryTimer.set(loop);
		store->lastPosCheckRetryTimer.set
			<DataStore, &DataStore::retryLastPositionCheck>
			(store.get());
		client->processing.insert(make_pair(id, store));
	}
	
	void unmarkClientProcessing(Client *client, const DataStoreId &id) {
		client->processing.erase(id);
	}
	
	void unmarkClientProcessing(Client *client, const DataStoreMap::iterator &it) {
		client->processing.erase(it);
	}
	
	void startLastPosCheckRetryTimer(DataStore *store, int retryCount) {
		store->lastPosCheckRetryTimer.set(
			5 * pow((double) 2, (double) retryCount),
			0.0);
		store->lastPosCheckRetryTimer.start();
	}
	
	void sendChangeNotification(Client *client, const DataStoreId &id) {
		StaticString args[] = { "changed", id.getGroupName(),
			id.getNodeName(), id.getCategory() };
		int count = sizeof(args) / sizeof(StaticString);
		
		char headerBuf[sizeof(uint16_t)];
		unsigned int outSize = ArrayMessage::outputSize(count);
		StaticString out[outSize];
		
		ArrayMessage::generate(args, count, headerBuf, out, outSize);
		client->write(out, outSize);
	}
	
	void sendErrorToClient(Client *client, const StaticString &message) {
		StaticString args[] = { "error", message };
		int count = sizeof(args) / sizeof(StaticString);
		
		char headerBuf[sizeof(uint16_t)];
		unsigned int outSize = ArrayMessage::outputSize(count);
		StaticString out[outSize];
		
		ArrayMessage::generate(args, count, headerBuf, out, outSize);
		client->write(out, outSize);
	}
	
	void processDoneMessage(Client *client, const vector<StaticString> &args) {
		if (OXT_UNLIKELY( args.size() != 4 )) {
			sendErrorToClient(client,
				"Invalid number of arguments for the 'done' message.");
			client->disconnect();
			return;
		}
		
		StaticString groupName = args[1];
		StaticString nodeName  = args[2];
		StaticString category  = args[3];
		StaticString lastPos   = args[4];
		
		DataStoreId identifier(groupName, nodeName, category);
		DataStoreMap::iterator it = client->processing.find(identifier);
		if (OXT_UNLIKELY( it == client->processing.end() )) {
			logError(client, "'done' message received for unregistered store");
			client->disconnect();
			return;
		}
		
		const DataStorePtr store = it->second;
		if (OXT_UNLIKELY( store->lastPosCheckRetryTimer.is_active() )) {
			logError(client, "Duplicate 'done' message received");
			client->disconnect();
			return;
		}
		
		if (!getLastPos) {
			unmarkClientProcessing(client, it);
			return;
		}
		
		string realLastPos;
		try {
			// Can return empty string.
			realLastPos = getLastPos(identifier.getGroupName(),
				identifier.getNodeName(),
				identifier.getCategory());
			if (realLastPos.empty()) {
				logError(client,
					"Cannot query last position for " +
					createHumanReadableIdentifier(store->identifier) +
					": unknown error");
			}
		} catch (const SystemException &e) {
			logSystemError(client,
				"Cannot query last position for " +
				createHumanReadableIdentifier(identifier) +
				": " + e.brief(),
				e.code());
		}
		
		if (realLastPos.empty()) {
			// Some error occurred. Try again later, with exponential backoff.
			store->lastPos.assign(lastPos.c_str(), lastPos.size());
			store->lastPosCheckRetry = 0;
			startLastPosCheckRetryTimer(store.get(), 0);
		} else if (lastPos == realLastPos) {
			unmarkClientProcessing(client, it);
		} else {
			sendChangeNotification(client, identifier);
		}
	}
	
	void retryLastPositionCheck(Client *client, DataStore *store) {
		string realLastPos;
		try {
			// Can return empty string.
			realLastPos = getLastPos(store->identifier.getGroupName(),
				store->identifier.getNodeName(),
				store->identifier.getCategory());
			if (realLastPos.empty()) {
				logError(client,
					"Cannot query last position for " +
					createHumanReadableIdentifier(store->identifier) +
					": unknown error");
			}
		} catch (const SystemException &e) {
			logSystemError(client,
				"Cannot query last position for " +
				createHumanReadableIdentifier(store->identifier) +
				": " + e.brief(),
				e.code());
		}
		
		if (realLastPos.empty()) {
			// Some error occurred. Try again later, with exponential backoff.
			store->lastPosCheckRetry++;
			if (store->lastPosCheckRetry > DataStore::MAX_RETRIES) {
				logError(client,
					"Giving up trying to query last position of " +
					createHumanReadableIdentifier(store->identifier) +
					" because too many errors occurred");
				unmarkClientProcessing(client, store->identifier);
			} else {
				startLastPosCheckRetryTimer(store, store->lastPosCheckRetry);
			}
		} else if (store->lastPos == realLastPos) {
			unmarkClientProcessing(client, store->identifier);
		} else {
			sendChangeNotification(client, store->identifier);
		}
	}
	
public:
	GetLastPosFunction getLastPos;
	
	ChangeNotifier(struct ev_loop *_loop) {
		loop = _loop;
	}
	
	~ChangeNotifier() {
		ClientSet::iterator it;
		ClientSet::iterator end = clients.end();
		
		for (it = clients.begin(); it != end; it++) {
			Client *client = *it;
			client->unref();
		}
		clients.clear();
	}
	
	void addClient(const FileDescriptor &fd) {
		Client *client = new Client(loop, fd);
		ScopeGuard guard(boost::bind(&ChangeNotifier::destroyClient, client));
		client->notifier     = this;
		client->onReadable   = onReadable;
		client->onDisconnect = onDisconnect;
		client->userData     = this;
		client->notifyReads(true);
		clients.insert(client);
		guard.clear();
	}
	
	void changed(const DataStoreId &dataStoreId) {
		ClientSet::iterator it;
		ClientSet::iterator end = clients.end();
		
		for (it = clients.begin(); it != end; it++) {
			Client *client = *it;
			if (clientIsProcessing(client, dataStoreId)) {
				markClientProcessing(client, dataStoreId);
				sendChangeNotification(client, dataStoreId);
			}
		}
	}
};


} // namespace Passenger

#endif /* _PASSENGER_CHANGE_NOTIFIER_H_ */
