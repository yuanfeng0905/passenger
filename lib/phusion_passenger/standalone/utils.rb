#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
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
end

end
end
