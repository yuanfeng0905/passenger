/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_UST_ROUTER_LOG_SINK_H_
#define _PASSENGER_UST_ROUTER_LOG_SINK_H_

#include <boost/shared_ptr.hpp>
#include <cstddef>
#include <ev++.h>
#include <jsoncpp/json.h>
#include <UstRouter/DataStoreId.h>
#include <Utils/JsonUtils.h>

namespace Passenger {
namespace UstRouter {

using namespace std;
using namespace boost;


class Controller;
inline struct ev_loop *Controller_getLoop(Controller *controller);


class LogSink {
public:
	Controller *controller;

	/**
	 * Marks how many times this LogSink is currently opened, i.e. the
	 * number of Transaction objects currently referencing this LogSink.
	 * Only when this value is 0 is this LogSink eligible for garbage
	 * collection.
	 */
	int opened;

	/**
	 * Last time append() was called. This may be 0, meaning that
	 * append() has never been called before.
	 */
	ev_tstamp lastWrittenTo;

	/**
	 * Last time the reference count on this log sink was decremented.
	 * A value of 0 means that this LogSink is new and the reference
	 * count has never been decremented before. Such LogSinks are not
	 * eligible for garbage collection.
	 */
	ev_tstamp lastClosed;

	/**
	 * Last time data was actually written to the underlying storage device.
	 * This may be 0, meaning that the data has never been flushed before.
	 */
	ev_tstamp lastFlushed;

	/**
	 * The amount of data that has been written to this sink so far.
	 */
	size_t totalBytesWritten;

	LogSink(Controller *_controller)
		: controller(_controller),
		  opened(0),
		  lastWrittenTo(0),
		  lastClosed(0),
		  lastFlushed(0),
		  totalBytesWritten(0)
		{ }

	virtual ~LogSink() {
		// Subclasses should flush any data in their destructors.
		// We cannot call flush() here because it is not allowed
		// to call virtual functions in a destructor.
	}

	virtual bool isRemote() const {
		return false;
	}

	virtual void append(const DataStoreId &dataStoreId,
		const StaticString &data)
	{
		lastWrittenTo = ev_now(Controller_getLoop(controller));
		totalBytesWritten += data.size();
	}

	virtual bool flush() {
		lastFlushed = ev_now(Controller_getLoop(controller));
		return true;
	}

	virtual Json::Value inspectStateAsJson() const {
		Json::Value doc;
		doc["opened"] = opened;
		if (lastWrittenTo == 0) {
			doc["last_written_to"] = Json::Value(Json::nullValue);
		} else {
			doc["last_written_to"] = timeToJson(lastWrittenTo * 1000000.0);
		}
		if (lastClosed == 0) {
			doc["last_closed"] = Json::Value(Json::nullValue);
		} else {
			doc["last_closed"] = timeToJson(lastClosed * 1000000.0);
		}
		if (lastFlushed == 0) {
			doc["last_flushed"] = Json::Value(Json::nullValue);
		} else {
			doc["last_flushed"] = timeToJson(lastFlushed * 1000000.0);
		}
		doc["total_bytes_written"] = byteSizeToJson(totalBytesWritten);
		return doc;
	}

	virtual string inspect() const = 0;
};

typedef boost::shared_ptr<LogSink> LogSinkPtr;


} // namespace UstRouter
} // namespace Passenger

#endif /* _PASSENGER_UST_ROUTER_LOG_SINK_H_ */
