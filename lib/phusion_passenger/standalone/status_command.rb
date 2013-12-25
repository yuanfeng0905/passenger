#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.
PhusionPassenger.require_passenger_lib 'standalone/command'

module PhusionPassenger
module Standalone

class StatusCommand < Command
	def self.description
		return "Show the status of a running Passenger Standalone instance."
	end
	
	def run
		parse_options!("status") do |opts|
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
			pid = @nginx.pid
		rescue SystemCallError, IOError
			running = false
		end
		if running
			puts "Phusion Passenger Standalone is running on PID #{pid}, according to PID file #{@options[:pid_file]}"
		else
			puts "Phusion Passenger Standalone is not running, according to PID file #{@options[:pid_file]}"
		end
	end
end

end
end
