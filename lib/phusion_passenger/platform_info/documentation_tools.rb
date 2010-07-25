#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'phusion_passenger/platform_info'

module PhusionPassenger

module PlatformInfo
	def self.asciidoc
		return find_command('asciidoc') || find_command('mizuho-asciidoc')
	end
	memoize :asciidoc
end

end # module PhusionPassenger
