/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2016 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
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
#include <Utils/StrIntUtils.h>
#include <Utils/SystemTime.h>
#include <Core/UnionStation/Transaction.h>

namespace Passenger {
namespace UnionStation {

using namespace std;
using namespace boost;


class StopwatchLog: public noncopyable {
private:
	Transaction * const transaction;
	const char *id;
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

	StopwatchLog(const TransactionPtr &_transaction, const char *id, const char *nameAndData)
		: transaction(_transaction.get())
	{
		this->id = id;
		ok = false;

		char message[250];
		char *pos = message;
		const char *end = message + sizeof(message);
		struct rusage usage;

		pos = appendData(pos, end, "BEGIN: ");
		pos = appendData(pos, end, id);
		pos = appendData(pos, end, " (");
		pos = appendData(pos, end, usecToString(SystemTime::getMonotonicUsec()));
		pos = appendData(pos, end, ",");
		if (getrusage(RUSAGE_SELF, &usage) == -1) {
			int e = errno;
			throw SystemException("getrusage() failed", e);
		}
		pos = appendData(pos, end, timevalToString(usage.ru_utime));
		pos = appendData(pos, end, ",");
		pos = appendData(pos, end, timevalToString(usage.ru_stime));
		pos = appendData(pos, end, ") ");

		if (nameAndData != NULL) {
			try {
				pos = appendData(pos, end, modp::b64_encode(nameAndData));
			} catch (const std::runtime_error &) {
				// non-fatal: ignore
			}
		}

		if (transaction != NULL) {
			transaction->message(StaticString(message, pos - message));
		}
	}

	~StopwatchLog() {
		if (transaction == NULL) {
			return;
		}
		char message[150];
		char *pos = message;
		const char *end = message + sizeof(message);
		struct rusage usage;

		if (ok) {
			pos = appendData(pos, end, "END: ");
		} else {
			pos = appendData(pos, end, "FAIL: ");
		}
		pos = appendData(pos, end, id);
		pos = appendData(pos, end, " (");
		pos = appendData(pos, end, usecToString(SystemTime::getMonotonicUsec()));
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
	}

	void success() {
		ok = true;
	}
};


} // namespace UnionStation
} // namespace Passenger

#endif /* _PASSENGER_UNION_STATION_STOPWATCH_LOG_H_ */
