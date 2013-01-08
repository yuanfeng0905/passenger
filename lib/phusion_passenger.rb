#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010, 2011, 2012 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
	###### Version numbers ######
	
	# Phusion Passenger version number. Don't forget to edit ext/common/Constants.h too.
	VERSION_STRING = '3.0.19'
	
	PREFERRED_NGINX_VERSION = '1.2.6'
	PREFERRED_PCRE_VERSION  = '8.31'
	STANDALONE_INTERFACE_VERSION  = 1
	
	ENTERPRISE_SERVER = true
	
	
	###### Directories ######
	# Don't forget to modify ext/common/ResourceLocator.h too.
	
	# Returns whether this Phusion Passenger installation is packaged
	# using the OS's native package management system, i.e. as opposed
	# to being installed from source or with RubyGems.
	def self.natively_packaged?
		if !defined?(@natively_packaged)
			@natively_packaged = !File.exist?("#{LIBDIR}/../Rakefile") ||
			                     !File.exist?("#{LIBDIR}/../DEVELOPERS.TXT")
		end
		return @natively_packaged
	end
	
	NATIVELY_PACKAGED_SOURCE_ROOT        = "/usr/share/phusion-passenger-enterprise/source"
	NATIVELY_PACKAGED_DOCDIR             = "/usr/share/doc/phusion-passenger-enterprise"
	NATIVELY_PACKAGED_RESOURCES_DIR      = "/usr/share/phusion-passenger-enterprise"
	NATIVELY_PACKAGED_AGENTS_DIR         = "/usr/lib/phusion-passenger-enterprise/agents"
	NATIVELY_PACKAGED_HELPER_SCRIPTS_DIR = "/usr/share/phusion-passenger-enterprise/helper-scripts"
	NATIVELY_PACKAGED_APACHE2_MODULE     = "/usr/lib/apache2/modules/mod_passenger.so"
	
	# Directory containing the Phusion Passenger Ruby libraries.
	LIBDIR         = File.expand_path(File.dirname(__FILE__))
	
	# Directory containing templates.
	TEMPLATES_DIR  = File.join(LIBDIR, "phusion_passenger", "templates")
	
	# Subdirectory under $HOME to use for storing resource files.
	LOCAL_DIR      = ".passenger-enterprise"
	
	# Directories in which to look for plugins.
	PLUGIN_DIRS    = ["/usr/share/phusion-passenger/plugins",
		"/usr/local/share/phusion-passenger/plugins",
		"~/.passenger/plugins"]
	
	# Directory under $HOME for storing Phusion Passenger Standalone resource files.
	LOCAL_STANDALONE_RESOURCE_DIR  = File.join(LOCAL_DIR, "standalone")
	
	# System-wide directory for storing Phusion Passenger Standalone resource files.
	GLOBAL_STANDALONE_RESOURCE_DIR = "/var/lib/passenger-enterprise-standalone"
	
	if !natively_packaged?
		# Top directory of the Phusion Passenger source code.
		SOURCE_ROOT        = File.expand_path(File.join(LIBDIR, ".."))
		
		# Documentation directory.
		DOCDIR             = File.join(SOURCE_ROOT, "doc")
		
		# Directory containing Phusion Passenger resource files.
		RESOURCES_DIR      = File.join(SOURCE_ROOT, "resources")
		
		AGENTS_DIR         = File.join(SOURCE_ROOT, "agents")
		
		HELPER_SCRIPTS_DIR = File.join(SOURCE_ROOT, "helper-scripts")
		
		# Location of the Apache 2 module.
		APACHE2_MODULE     = File.join(SOURCE_ROOT, "ext", "apache2", "mod_passenger.so")
		
		# Directory possibly containing #{archdir}/passenger_native_support.so.
		# Not available when natively packaged.
		NATIVE_SUPPORT_DIR = File.join(SOURCE_ROOT, "ext", "ruby")
	else
		SOURCE_ROOT        = NATIVELY_PACKAGED_SOURCE_ROOT
		DOCDIR             = NATIVELY_PACKAGED_DOCDIR
		RESOURCES_DIR      = NATIVELY_PACKAGED_RESOURCES_DIR
		AGENTS_DIR         = NATIVELY_PACKAGED_AGENTS_DIR
		HELPER_SCRIPTS_DIR = NATIVELY_PACKAGED_HELPER_SCRIPTS_DIR
		APACHE2_MODULE     = NATIVELY_PACKAGED_APACHE2_MODULE
	end
	
	
	###### Other resource locations ######
	
	STANDALONE_BINARIES_URL_ROOT  = nil
	
	
	if !$LOAD_PATH.include?(LIBDIR)
		$LOAD_PATH.unshift(LIBDIR)
		$LOAD_PATH.uniq!
	end
end if !defined?(PhusionPassenger::LIBDIR)
