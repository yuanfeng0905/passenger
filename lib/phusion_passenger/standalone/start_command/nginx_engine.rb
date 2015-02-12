#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'erb'
require 'etc'
PhusionPassenger.require_passenger_lib 'constants'
PhusionPassenger.require_passenger_lib 'platform_info/ruby'
PhusionPassenger.require_passenger_lib 'standalone/control_utils'
PhusionPassenger.require_passenger_lib 'utils/tmpio'
PhusionPassenger.require_passenger_lib 'utils/shellwords'

module PhusionPassenger
module Standalone
class StartCommand

module NginxEngine
private
	def start_engine_real
		Standalone::ControlUtils.require_daemon_controller
		@engine = DaemonController.new(build_daemon_controller_options)
		write_nginx_config_file(nginx_config_path)

		begin
			@engine.start
		rescue DaemonController::AlreadyStarted
			begin
				pid = @engine.pid
			rescue SystemCallError, IOError
				pid = nil
			end
			if pid
				abort "#{PROGRAM_NAME} Standalone is already running on PID #{pid}."
			else
				abort "#{PROGRAM_NAME} Standalone is already running."
			end
		rescue DaemonController::StartError => e
			abort "Could not start the Nginx engine:\n#{e}"
		end
	end

	def build_daemon_controller_options
		if @options[:socket_file]
			ping_spec = [:unix, @options[:socket_file]]
		else
			ping_spec = [:tcp, @options[:address], @options[:port]]
		end
		return {
			:identifier    => 'Nginx',
			:start_command => "#{@nginx_binary} " +
				"-c #{Shellwords.escape nginx_config_path} " +
				"-p #{Shellwords.escape @working_dir}",
			:ping_command  => ping_spec,
			:pid_file      => @options[:pid_file],
			:log_file      => @options[:log_file],
			:timeout       => 25
		}
	end

	def nginx_config_path
		return "#{@working_dir}/nginx.conf"
	end

	def write_nginx_config_file(path)
		File.open(path, 'w') do |f|
			f.chmod(0644)
			erb = ERB.new(File.read(nginx_config_template_filename), nil, "-")
			erb.filename = nginx_config_template_filename
			current_user = Etc.getpwuid(Process.uid).name

			# The template requires some helper methods which are defined in start_command.rb.
			output = erb.result(binding)
			f.write(output)
			puts output if debugging?
		end
	end

	def nginx_config_template_filename
		if @options[:nginx_config_template]
			return @options[:nginx_config_template]
		else
			return File.join(PhusionPassenger.resources_dir,
				"templates", "standalone", "config.erb")
		end
	end

	def debugging?
		return ENV['PASSENGER_DEBUG'] && !ENV['PASSENGER_DEBUG'].empty?
	end

	#### Config file template helpers ####

	def nginx_listen_address(options = @options)
		if options[:socket_file]
			return "unix:" + File.absolute_path_no_resolve(options[:socket_file])
		else
			return compose_ip_and_port(options[:address], options[:port])
		end
	end

	def nginx_listen_address_with_ssl_port(options = @options)
		if options[:socket_file]
			return "unix:" + File.absolute_path_no_resolve(options[:socket_file])
		else
			return compose_ip_and_port(options[:address], options[:ssl_port])
		end
	end

	def default_group_for(username)
		user = Etc.getpwnam(username)
		group = Etc.getgrgid(user.gid)
		return group.name
	end

	def nginx_option(nginx_config_name, option_name)
		if @options[option_name]
			return "#{nginx_config_name} #{@options[option_name]};"
		end
	end

	def default_group_for(username)
		user = Etc.getpwnam(username)
		group = Etc.getgrgid(user.gid)
		return group.name
	end

	def boolean_config_value(val)
		return val ? "on" : "off"
	end

	def serialize_strset(*items)
		if "".respond_to?(:force_encoding)
			items = items.map { |x| x.force_encoding('binary') }
			null  = "\0".force_encoding('binary')
		else
			null  = "\0"
		end
		return [items.join(null)].pack('m*').gsub("\n", "").strip
	end

	#####################

	def reload_engine(pid)
		write_nginx_config_file(nginx_config_path)
		Process.kill('HUP', pid) rescue nil
	end
end

end # module StartCommand
end # module Standalone
end # module PhusionPassenger
