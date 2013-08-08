/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010, 2011, 2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_RESOURCE_LOCATOR_H_
#define _PASSENGER_RESOURCE_LOCATOR_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include <Exceptions.h>
#include <Utils.h>
#include <Utils/IniFile.h>

namespace Passenger {

using namespace std;
using namespace boost;


/**
 * Locates various Phusion Passenger resources on the filesystem. All Phusion Passenger
 * files are located through this class. There's similar code in lib/phusion_passenger.rb.
 * See doc/Packaging.txt.md for an introduction about where Phusion Passenger expects its
 * files to be located.
 */
class ResourceLocator {
private:
	string root;
	string binDir;
	string agentsDir;
	string helperScriptsDir;
	string resourcesDir;
	string docDir;
	string rubyLibDir;
	
	static string getOption(const string &file, const IniFileSectionPtr &section, const string &key) {
		if (section->hasKey(key)) {
			return section->get(key);
		} else {
			throw RuntimeException("Option '" + key + "' missing in file '" + file + "'");
		}
	}
	
public:
	ResourceLocator(const string &rootOrFile) {
		root = rootOrFile;
		if (getFileType(rootOrFile) == FT_REGULAR) {
			string file = rootOrFile;
			IniFileSectionPtr options = IniFile(file).section("locations");
			binDir              = getOption(file, options, "bin_dir");
			agentsDir           = getOption(file, options, "agents_dir");
			helperScriptsDir    = getOption(file, options, "helper_scripts_dir");
			resourcesDir        = getOption(file, options, "resources_dir");
			docDir              = getOption(file, options, "doc_dir");
			rubyLibDir          = getOption(file, options, "ruby_libdir");
		} else {
			string root = rootOrFile;
			binDir              = root + "/bin";
			agentsDir           = root + "/buildout/agents";
			helperScriptsDir    = root + "/helper-scripts";
			resourcesDir        = root + "/resources";
			docDir              = root + "/doc";
			rubyLibDir          = root + "/lib";
		}
	}
	
	string getRoot() const {
		return root;
	}
	
	string getAgentsDir() const {
		return agentsDir;
	}
	
	string getHelperScriptsDir() const {
		return helperScriptsDir;
	}
	
	string getResourcesDir() const {
		return resourcesDir;
	}
	
	string getDocDir() const {
		return docDir;
	}
	
	// Can be empty.
	string getRubyLibDir() const {
		return rubyLibDir;
	}
};

typedef shared_ptr<ResourceLocator> ResourceLocatorPtr;


}

#endif /* _PASSENGER_RESOURCE_LOCATOR_H_ */
