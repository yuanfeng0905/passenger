#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.
PhusionPassenger.require_passenger_lib 'standalone/command'

module PhusionPassenger
module Standalone

class Main
	COMMANDS = [
		['start',   'StartCommand'],
		['stop',    'StopCommand'],
		['status',  'StatusCommand'],
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
			PhusionPassenger.require_passenger_lib "standalone/#{filename}"
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
		return !!find_command_spec(name)
	end
	
	def run_command(name, args = [])
		if spec = find_command_spec(name)
			klass = get_command_class(spec)
			klass.require_libs if klass.respond_to?(:require_libs)
			klass.new(args).run
		else
			raise ArgumentError, "Command '#{name}' doesn't exist"
		end
	end

	def find_command_spec(name)
		COMMANDS.each do |spec|
			if spec[0] == name
				return spec
			end
		end
		return nil
	end

	def get_command_class(spec)
		command_name, class_name = spec
		filename = command_name.sub(/-/, '_') + "_command"
		PhusionPassenger.require_passenger_lib("standalone/#{filename}")
		return Standalone.const_get(class_name)
	end
end

end # module Standalone
end # module PhusionPassenger
