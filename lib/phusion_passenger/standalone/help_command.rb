#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.
PhusionPassenger.require_passenger_lib 'standalone/command'

module PhusionPassenger
module Standalone

class HelpCommand < Command
	def self.show_in_command_list
		return false
	end
	
	def run
		puts "Phusion Passenger Standalone, the easiest way to deploy web apps."
		puts
		puts "Available commands:"
		puts
		Main.each_command do |command_name, command_class|
			if command_class.show_in_command_list
				printf "  passenger %-15s  %s\n",
					command_name,
					wrap_desc(command_class.description, 51, 29)
			end
		end
		puts
		puts "Special options:"
		puts
		puts "  passenger --help      Display this help message."
		puts "  passenger --version   Display version number."
		puts
		puts "For more information about a specific command, please type"
		puts "'passenger <COMMAND> --help', e.g. 'passenger start --help'."
	end
end

end # module Standalone
end # module PhusionPassenger
