#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'phusion_passenger/platform_info'

module PhusionPassenger

module PlatformInfo
	def self.curl_flags
		result = `(curl-config --cflags) 2>/dev/null`.strip
		if result.empty?
			return nil
		else
			return result
		end
	end
	memoize :curl_flags
	
	def self.curl_libs
		result = `(curl-config --libs) 2>/dev/null`.strip
		if result.empty?
			return nil
		else
			return result
		end
	end
	memoize :curl_libs
	
	def self.curl_supports_ssl?
		features = `(curl-config --features) 2>/dev/null`
		return features =~ /SSL/
	end
	memoize :curl_supports_ssl?
end

end # module PhusionPassenger
