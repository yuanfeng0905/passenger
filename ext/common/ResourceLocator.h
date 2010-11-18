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
#include <IniFile.h>
#include <Exceptions.h>
#include <Utils.h>

namespace Passenger {

using namespace boost;


/**
 * Locates various Phusion Passenger resources on the filesystem.
 */
class ResourceLocator {
private:
	string agentsDir;
	string helperScriptsDir;
	string resourcesDir;
	string docDir;
	string rubyLibDir;
	string compilableSourceDir;
	string apache2Module;
	
	string getOption(const string &file, const IniFileSectionPtr &section, const string &key) const {
		if (section->hasKey(key)) {
			return section->get(key);
		} else {
			throw RuntimeException("Option '" + key + "' missing in file " + file);
		}
	}
	
public:
	ResourceLocator(const string &rootOrFile) {
		if (getFileType(rootOrFile) == FT_DIRECTORY) {
			string root = rootOrFile;
			bool nativelyPackaged = !fileExists(root + "/Rakefile") ||
				!fileExists(root + "/DEVELOPERS.TXT");
			
			if (nativelyPackaged) {
				agentsDir           = "/usr/lib/phusion-passenger-enterprise/agents";
				helperScriptsDir    = "/usr/share/phusion-passenger-enterprise/helper-scripts";
				resourcesDir        = "/usr/share/phusion-passenger-enterprise";
				docDir              = "/usr/share/doc/phusion-passenger-enterprise";
				rubyLibDir          = "";
				compilableSourceDir = "/usr/share/phusion-passenger-enterprise/compilable-source";
				apache2Module       = "/usr/lib/apache2/modules/mod_passenger.so";
			} else {
				agentsDir           = root + "/agents";
				helperScriptsDir    = root + "/helper-scripts";
				resourcesDir        = root + "/resources";
				docDir              = root + "/doc";
				rubyLibDir          = root + "/lib";
				compilableSourceDir = root;
				apache2Module       = root + "ext/apache2/mod_passenger.so";
			}
			
		} else {
			string file = rootOrFile;
			IniFileSectionPtr options = IniFile(file).section("locations");
			agentsDir           = getOption(file, options, "agents");
			helperScriptsDir    = getOption(file, options, "helper_scripts");
			resourcesDir        = getOption(file, options, "resources");
			docDir              = getOption(file, options, "doc");
			rubyLibDir          = getOption(file, options, "rubylib");
			compilableSourceDir = getOption(file, options, "compilable_source");
			apache2Module       = getOption(file, options, "apache2_module");
		}
	}
	
	string getAgentsDir() const {
		return agentsDir;
	}
	
	string getHelperScriptsDir() const {
		return helperScriptsDir;
	}
	
	string getSpawnServerFilename() const {
		return getHelperScriptsDir() + "/passenger-spawn-server";
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
	
	string getCompilableSourceDir() const {
		return compilableSourceDir;
	}
	
	string getApache2ModuleFilename() const {
		return apache2Module;
	}
};


}

#endif /* _PASSENGER_RESOURCE_LOCATOR_H_ */
