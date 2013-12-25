#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.
PhusionPassenger.require_passenger_lib 'standalone/command'

module PhusionPassenger
module Standalone

class StopCommand < Command
	def self.description
		return "Stop a running Phusion Passenger Standalone instance."
	end
	
	def run
		parse_options!("stop") do |opts|
			opts.on("-p", "--port NUMBER", Integer,
				wrap_desc("The port number of a Phusion Passenger Standalone instance (default: #{@options[:port]})")) do |value|
				@options[:port] = value
			end
			opts.on("--pid-file FILE", String,
				wrap_desc("PID file of a running Phusion Passenger Standalone instance.")) do |value|
				@options[:pid_file] = value
			end
		end
		
		determine_various_resource_locations(false)
		create_nginx_controller
		begin
			running = @nginx.running?
		rescue SystemCallError, IOError
			running = false
		end
		if running
			@nginx.stop
		else
			STDERR.puts "According to the PID file '#{@options[:pid_file]}', " <<
				"Phusion Passenger Standalone doesn't seem to be running."
			STDERR.puts
			STDERR.puts "If you know that Phusion Passenger Standalone *is* running then one of these"
			STDERR.puts "might be the cause of this error:"
			STDERR.puts
			STDERR.puts " * The Phusion Passenger Standalone instance that you want to stop isn't running"
			STDERR.puts "   on port #{@options[:port]}, but on another port. If this is the case then you"
			STDERR.puts "   should specify the right port with --port."
			STDERR.puts "   If the instance is listening on a Unix socket file instead of a TCP port,"
			STDERR.puts "   then please specify the PID file's filename with --pid-file."
			STDERR.puts " * The instance that you want to stop has stored its PID file in a non-standard"
			STDERR.puts "   location. In this case please specify the right PID file with --pid-file."
			exit 1
		end
	end
end

end
end
