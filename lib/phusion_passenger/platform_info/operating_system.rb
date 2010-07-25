#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'rbconfig'
require 'phusion_passenger/platform_info'

module PhusionPassenger

module PlatformInfo
	# Returns the operating system's name. This name is in lowercase and contains no spaces,
	# and thus is suitable to be used in some kind of ID. E.g. "linux", "macosx".
	def self.os_name
		if Config::CONFIG['target_os'] =~ /darwin/ && (sw_vers = find_command('sw_vers'))
			return "macosx"
		else
			return RUBY_PLATFORM.sub(/.*?-/, '')
		end
	end
	memoize :os_name
	
	# The current platform's shared library extension ('so' on most Unices).
	def self.library_extension
		if RUBY_PLATFORM =~ /darwin/
			return "bundle"
		else
			return "so"
		end
	end
end

end # module PhusionPassenger
