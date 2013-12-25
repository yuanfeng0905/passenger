#  Phusion Passenger - https://www.phusionpassenger.com/
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
		if !defined?(PlatformInfo) || !PlatformInfo.respond_to?(:cxx_binary_compatibility_id)
			PhusionPassenger.require_passenger_lib 'platform_info/binary_compatibility'
		end
	end
	
	def runtime_version_string(nginx_version)
		if PhusionPassenger.originally_packaged? || nginx_version != PhusionPassenger::PREFERRED_NGINX_VERSION
			require_platform_info_binary_compatibility
			return "#{VERSION_STRING}-#{PlatformInfo.passenger_binary_compatibility_id}"
		else
			return VERSION_STRING
		end
	end
end

end
end
