/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_UTILS_JSON_UTILS_H_
#define _PASSENGER_UTILS_JSON_UTILS_H_

#include <string>
#include <StaticString.h>
#include <Utils/json.h>
#include <Utils/StrIntUtils.h>

namespace Passenger {

using namespace std;

inline string
stringifyJson(const Json::Value &value) {
	Json::FastWriter writer;
	string str = writer.write(value);
	str.erase(str.size() - 1, 1);
	return str;
}

/** `str` MUST be NULL-terminated! */
inline string
jsonString(const Passenger::StaticString &str) {
	return stringifyJson(Json::Value(Json::StaticString(str.data())));
}

inline Json::Value
timeToJson(unsigned long long timestamp) {
	Json::Value doc;
	time_t time = (time_t) timestamp / 1000000;
	char buf[32];
	size_t len;

	doc["timestamp"] = timestamp / (double) 1000000;

	ctime_r(&time, buf);
	len = strlen(buf);
	if (len > 0) {
		// Get rid of trailing newline
		buf[len - 1] = '\0';
	}
	doc["local"] = buf;
	doc["relative"] = distanceOfTimeInWords(time) + " ago";

	return doc;
}

} // namespace Passenger

#endif /* _PASSENGER_UTILS_JSON_UTILS_H_ */
