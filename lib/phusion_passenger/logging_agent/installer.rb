#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'optparse'
require 'fileutils'
require 'phusion_passenger/platform_info/ruby'
require 'phusion_passenger/dependencies'
require 'phusion_passenger/abstract_installer'

module PhusionPassenger
module LoggingAgent

class Installer < PhusionPassenger::AbstractInstaller
	def dependencies
		return [
			Dependencies::GCC,
			Dependencies::Make,
			Dependencies::Curl_Dev,
			Dependencies::Zlib_Dev
		]
	end
	
	def install!
		Dir.chdir(SOURCE_ROOT)
		check_dependencies || exit(1)
		check_whether_we_can_write_to(SOURCE_ROOT) || exit(1)
		compile || exit(1)
	end

	def before_install
		super
		@old_working_dir = Dir.pwd
	end
	
	def after_install
		super
		Dir.chdir(@old_working_dir)
	end

private
	def compile
		new_screen
		color_puts "<banner>Compiling...</banner>"
		return sh("#{PlatformInfo.rake_command} agents/PassengerLoggingAgent")
	end
	
	def check_whether_we_can_write_to(dir)
		FileUtils.mkdir_p(dir)
		File.new("#{dir}/__test__.txt", "w").close
		return true
	rescue
		new_screen
		if Process.uid == 0
			render_template 'nginx/cannot_write_to_dir', :dir => dir
		else
			render_template 'nginx/run_installer_as_root', :dir => dir
		end
		return false
	ensure
		File.unlink("#{dir}/__test__.txt") rescue nil
	end
end

end
end
