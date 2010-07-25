#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.
require 'phusion_passenger'
require 'phusion_passenger/standalone/command'

module PhusionPassenger
module Standalone

class VersionCommand < Command
	def self.show_in_command_list
		return false
	end
	
	def run
		puts "Phusion Passenger version #{VERSION_STRING}"
		puts
		puts '"Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.'
	end
end

end
end
