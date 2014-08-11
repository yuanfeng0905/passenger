/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011, 2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_APPLICATION_POOL2_COMPONENT_INFO_H_
#define _PASSENGER_APPLICATION_POOL2_COMPONENT_INFO_H_

#include <string>

namespace Passenger {
namespace ApplicationPool2 {

using namespace std;


struct ComponentInfo {
	string name;
	bool   isDefault;

	/****************/
	/****************/

	ComponentInfo() {
		isDefault = false;
		/******************/
	}
};


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL2_COMPONENT_INFO_H_ */
