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

#include <boost/function.hpp>
#include <string>
#include <ev++.h>
#include "DataStoreId.h"
#include "../EventedClient.h"
#include "../FileDescriptor.h"
#include "../StaticString.h"

namespace Passenger {

using namespace std;
using namespace boost;


class ChangeNotifier {
public:
	typedef function<string (const StaticString &groupName, const StaticString &nodeName,
		const StaticString &category)> GetLastPosFunction;
	
	GetLastPosFunction getLastPos;
	
	ChangeNotifier(struct ev_loop *_loop) { }
	
	void addClient(const FileDescriptor &fd) { }
	
	void changed(const DataStoreId &dataStoreId) { }
};


} // namespace Passenger

#endif /* _PASSENGER_CHANGE_NOTIFIER_H_ */
