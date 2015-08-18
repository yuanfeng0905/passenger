/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2014 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_UNION_STATION_STOPWATCH_LOG_H_
#define _PASSENGER_UNION_STATION_STOPWATCH_LOG_H_

#include <boost/noncopyable.hpp>

#include <sys/resource.h>

#include <string>

#include <StaticString.h>
#include <Exceptions.h>
#include <UnionStation/Transaction.h>
#include <Utils/StrIntUtils.h>
#include <Utils/SystemTime.h>

namespace Passenger {
namespace UnionStation {

using namespace std;
using namespace boost;


class StopwatchLog: public noncopyable {
private:
	Transaction * const transaction;
	union {
		const char *name;
		struct {
			const char *endMessage;
			const char *abortMessage;
		} granular;
	} data;
	enum {
		NAME,
		GRANULAR
	} type: 1;
	bool ok;

	static string timevalToString(struct timeval &tv) {
		unsigned long long i = (unsigned long long) tv.tv_sec * 1000000 + tv.tv_usec;
		return usecToString(i);
	}

	static string usecToString(unsigned long long usec) {
		char timestamp[2 * sizeof(unsigned long long) + 1];
		integerToHexatri<unsigned long long>(usec, timestamp);
		return timestamp;
	}

public:
	StopwatchLog()
		: transaction(NULL)
		{ }

	StopwatchLog(const TransactionPtr &_transaction, const char *name)
		: transaction(_transaction.get())
	{
		type = NAME;
		data.name = name;
		ok = false;

		char message[150];
		char *pos = message;
		const char *end = message + sizeof(message);
		struct rusage usage;

		pos = appendData(pos, end, "BEGIN: ");
		pos = appendData(pos, end, name);
		pos = appendData(pos, end, " (");
		pos = appendData(pos, end, usecToString(SystemTime::getUsec()));
		pos = appendData(pos, end, ",");
		if (getrusage(RUSAGE_SELF, &usage) == -1) {
			int e = errno;
			throw SystemException("getrusage() failed", e);
		}
		pos = appendData(pos, end, timevalToString(usage.ru_utime));
		pos = appendData(pos, end, ",");
		pos = appendData(pos, end, timevalToString(usage.ru_stime));
		pos = appendData(pos, end, ") ");

		if (transaction != NULL) {
			transaction->message(StaticString(message, pos - message));
		}
	}

	StopwatchLog(const TransactionPtr &_transaction,
		const char *beginMessage,
		const char *endMessage,
		const char *abortMessage = NULL)
		: transaction(_transaction.get())
	{
		if (_transaction != NULL) {
			type = GRANULAR;
			data.granular.endMessage = endMessage;
			data.granular.abortMessage = abortMessage;
			ok = abortMessage == NULL;
			_transaction->message(beginMessage);
		}
	}

	~StopwatchLog() {
		if (transaction == NULL) {
			return;
		}
		if (type == NAME) {
			char message[150];
			char *pos = message;
			const char *end = message + sizeof(message);
			struct rusage usage;

			if (ok) {
				pos = appendData(pos, end, "END: ");
			} else {
				pos = appendData(pos, end, "FAIL: ");
			}
			pos = appendData(pos, end, data.name);
			pos = appendData(pos, end, " (");
			pos = appendData(pos, end, usecToString(SystemTime::getUsec()));
			pos = appendData(pos, end, ",");
			if (getrusage(RUSAGE_SELF, &usage) == -1) {
				int e = errno;
				throw SystemException("getrusage() failed", e);
			}
			pos = appendData(pos, end, timevalToString(usage.ru_utime));
			pos = appendData(pos, end, ",");
			pos = appendData(pos, end, timevalToString(usage.ru_stime));
			pos = appendData(pos, end, ")");

			transaction->message(StaticString(message, pos - message));
		} else {
			if (ok) {
				transaction->message(data.granular.endMessage);
			} else {
				transaction->message(data.granular.abortMessage);
			}
		}
	}

	void success() {
		ok = true;
	}
};


} // namespace UnionStation
} // namespace Passenger

#endif /* _PASSENGER_UNION_STATION_STOPWATCH_LOG_H_ */
