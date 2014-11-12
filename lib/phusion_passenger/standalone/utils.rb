#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
module Standalone

module Utils
	extend self    # Make methods available as class methods.

	def self.included(klass)
		# When included into another class, make sure that Utils
		# methods are made private.
		public_instance_methods(false).each do |method_name|
			klass.send(:private, method_name)
		end
	end

	def require_platform_info_binary_compatibility
		if !defined?(PlatformInfo) || !PlatformInfo.respond_to?(:cxx_binary_compatibility_id)
			PhusionPassenger.require_passenger_lib 'platform_info/binary_compatibility'
		end
	end
end

end
end
