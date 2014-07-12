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

} // namespace Passenger

#endif /* _PASSENGER_UTILS_JSON_UTILS_H_ */
