/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_RESOURCE_LOCATOR_H_
#define _PASSENGER_RESOURCE_LOCATOR_H_

#include <string>
#include "Utils.h"

namespace Passenger {

class ResourceLocator {
private:
	string root;
	bool nativelyPackaged;
	
public:
	ResourceLocator(const string &passengerRoot) {
		root = passengerRoot;
		nativelyPackaged = !fileExists(root + "/Rakefile") ||
			!fileExists(root + "/DEVELOPERS.TXT");
	}
	
	string getSourceRoot() const {
		if (nativelyPackaged) {
			return "/usr/lib/phusion-passenger-enterprise/source";
		} else {
			return root;
		}
	}
	
	string getAgentsDir() const {
		if (nativelyPackaged) {
			return "/usr/lib/phusion-passenger-enterprise/agents";
		} else {
			return root + "/agents";
		}
	}
	
	string getHelperScriptsDir() const {
		if (nativelyPackaged) {
			return "/usr/share/phusion-passenger-enterprise/helper-scripts";
		} else {
			return root + "/helper-scripts";
		}
	}
	
	string getSpawnServerFilename() const {
		return getHelperScriptsDir() + "/passenger-spawn-server";
	}
	
	string getCertificatesDir() const {
		if (nativelyPackaged) {
			return "/usr/share/phusion-passenger/certificates";
		} else {
			return root + "/misc/certificates";
		}
	}
};

}

#endif /* _PASSENGER_RESOURCE_LOCATOR_H_ */
