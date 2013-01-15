#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.
require 'phusion_passenger/standalone/command'

module PhusionPassenger
module Standalone

class Main
	COMMANDS = [
		['start',   'StartCommand'],
		['stop',    'StopCommand'],
		['status',  'StatusCommand'],
		['package-runtime', 'PackageRuntimeCommand'],
		['version', 'VersionCommand'],
		['help',    'HelpCommand']
	]
	
	def self.run!(argv)
		new.run!(argv)
	end
	
	def self.each_command
		COMMANDS.each do |command_spec|
			command_name = command_spec[0]
			filename     = command_name.sub(/-/, '_') + "_command"
			require "phusion_passenger/standalone/#{filename}"
			command_class = Standalone.const_get(command_spec[1])
			yield(command_name, command_class)
		end
	end
	
	def run!(argv)
		command = argv[0]
		if command.nil? || command == '-h' || command == '--help'
			run_command('help')
			exit
		elsif command == '-v' || command == '--version'
			run_command('version')
			exit
		elsif command_exists?(command)
			begin
				run_command(command, argv[1..-1])
			rescue => e
				if defined?(OptionParser::ParseError) && e.is_a?(OptionParser::ParseError)
					puts e
					puts
					puts "Please see '--help' for valid options."
					exit 1
				elsif defined?(ConfigFile::DisallowedContextError) && e.is_a?(ConfigFile::DisallowedContextError)
					puts "*** Error in #{e.filename} line #{e.line}:"
					puts e
					exit 1
				else
					raise e
				end
			end
		else
			STDERR.puts "Unknown command '#{command}'. Please type --help for options."
			exit 1
		end
	end

private
	def command_exists?(name)
		return COMMANDS.any? do |element|
			element[0] == name
		end
	end
	
	def run_command(name, args = [])
		Main.each_command do |command_name, command_class|
			if command_name == name
				return command_class.new(args).run
			end
		end
		raise ArgumentError, "Command '#{name}' doesn't exist"
	end
end

end # module Standalone
end # module PhusionPassenger
