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

	def runtime_version_string(nginx_version)
		if PhusionPassenger.originally_packaged? || nginx_version != PhusionPassenger::PREFERRED_NGINX_VERSION
			require_platform_info_binary_compatibility
			return "#{VERSION_STRING}-#{PlatformInfo.passenger_binary_compatibility_id}"
		else
			return VERSION_STRING
		end
	end

	# Dir.pwd resolves symlinks. So in turn, File.expand_path/File.absolute_path
	# do that too. We work around that by shelling out to the `pwd` tool.
	if File.respond_to?(:absolute_path)
		def absolute_path(path)
			return File.absolute_path(path, `pwd`.strip)
		end
	else
		def absolute_path(path)
			return File.expand_path(path, `pwd`.strip)
		end
	end
end

end
end
