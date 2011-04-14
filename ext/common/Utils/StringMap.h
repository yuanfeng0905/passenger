/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2011 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_STRING_MAP_H_
#define _PASSENGER_STRING_MAP_H_

#include <string>
#include <map>
#include <utility>

#include <StaticString.h>

namespace Passenger {

using namespace std;


/**
 * An efficient map with string keys. map<string, T> forces one to construct an
 * std::string object when looking up the map. StringMap interns all keys and
 * allows lookups without constructing an std::string key.
 *
 * StringMap requires the following properties on T:
 * - T's default constructor must be cheap, otherwise set() can be a bit slow.
 * - T must support operator=().
 */
template<typename T>
class StringMap {
private:
	struct Entry {
		string key;
		T value;
	};
	
	typedef map<StaticString, Entry> InternalMap;
	typedef typename InternalMap::iterator Iterator;
	typedef typename InternalMap::const_iterator ConstIterator;
	typedef typename InternalMap::value_type ValueType;
	InternalMap store;
	
public:
	T get(const StaticString &key) const {
		ConstIterator it = store.find(key);
		if (it == store.end()) {
			return T();
		} else {
			return it->second.value;
		}
	}
	
	bool set(const StaticString &key, const T &value) {
		pair<Iterator, bool> result = store.insert(make_pair(key, Entry()));
		if (result.second) {
			// Key has been inserted. Copy it internally and point key
			// to the copy.
			ValueType &node = *result.first;
			StaticString &originalKey = const_cast<StaticString &>(node.first);
			Entry &entry = node.second;
			entry.key = key;
			entry.value = value;
			originalKey = entry.key;
			return true;
		} else {
			// Key already exists. Update value.
			Entry &entry = result.first->second;
			entry.value = value;
			return false;
		}
	}
	
	bool remove(const StaticString &key) {
		return store.erase(key) > 0;
	}
};


} // namespace Passenger

#endif /* _PASSENGER_STRING_MAP_H_ */
