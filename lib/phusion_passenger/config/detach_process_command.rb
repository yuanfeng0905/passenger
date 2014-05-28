#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'optparse'
PhusionPassenger.require_passenger_lib 'constants'
PhusionPassenger.require_passenger_lib 'admin_tools/server_instance'
PhusionPassenger.require_passenger_lib 'config/command'
PhusionPassenger.require_passenger_lib 'config/utils'

module PhusionPassenger
module Config

class DetachProcessCommand < Command
	include PhusionPassenger::Config::Utils

	def run
		parse_options
		select_passenger_instance
		@admin_client = connect_to_passenger_admin_socket(:role => :passenger_status)
		perform_detach
	end

private
	def self.create_option_parser(options)
		OptionParser.new do |opts|
			nl = "\n" + ' ' * 37
			opts.banner = "Usage: passenger-config detach-process [OPTIONS] <PID>\n"
			opts.separator ""
			opts.separator "  Remove an application process from the #{PROGRAM_NAME} process pool."
			opts.separator "  Has a similar effect to killing the application process directly with"
			opts.separator "  `kill <PID>`, but killing directly may cause the HTTP client to see an"
			opts.separator "  error, while using this command guarantees that clients see no errors."
			opts.separator ""

			opts.separator "Options:"
			opts.on("--instance INSTANCE_PID", Integer, "The #{PROGRAM_NAME} instance to select") do |value|
				options[:instance] = value
			end
			opts.on("-h", "--help", "Show this help") do
				options[:help] = true
			end
		end
	end

	def help
		puts @parser
	end

	def parse_options
		super
		if @argv.empty?
			abort "Please pass a PID. " +
				"See --help for more information."
		elsif @argv.size == 1
			@pid = @argv[0].to_i
		elsif @argv.size > 1
			help
			abort
		end
	end

	def perform_detach
		if @admin_client.pool_detach_process(@pid)
			puts "Process #{@pid} detached."
		else
			abort "Could not detach process #{@pid}."
		end
	end
end

end # module Config
end # module PhusionPassenger
