#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
module Standalone

module Utils
private
	def require_platform_info_binary_compatibility
		if !defined?(PlatformInfo) || !PlatformInfo.respond_to?(:cpu_architecture)
			require 'phusion_passenger/platform_info/binary_compatibility'
		end
	end
	
	def runtime_version_string
		require_platform_info_binary_compatibility
		return "#{VERSION_STRING}-#{PlatformInfo.passenger_binary_compatibility_id}"
	end
end

end
end