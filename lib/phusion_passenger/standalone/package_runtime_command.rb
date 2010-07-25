#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.
require 'phusion_passenger/standalone/command'

module PhusionPassenger
module Standalone

class PackageRuntimeCommand < Command
	def self.description
		return "Package the Phusion Passenger Standalone runtime."
	end
	
	def run
		destdir = File.expand_path("passenger-standalone")
		description =
			"Package the Phusion Passenger Standalone runtime into the specified directory. If\n" <<
			"DIRECTORY is not given then #{destdir} will be used."
		parse_options!("package [directory]", description) do |opts|
			opts.on("--nginx-version VERSION", String,
				wrap_desc("Nginx version to use as core (default: #{@options[:nginx_version]})")) do |value|
				@options[:nginx_version] = value
			end
			opts.on("--nginx-tarball FILENAME", String,
				wrap_desc("Use the given tarball instead of downloading from the Internet")) do |value|
				@options[:nginx_tarball] = value
			end
		end
		
		destdir     = File.expand_path(@args[0]) if @args[0]
		runtime_dir = "#{destdir}/#{runtime_version_string}"
		support_dir = "#{runtime_dir}/support"
		nginx_dir   = "#{runtime_dir}/nginx-#{@options[:nginx_version]}"
		
		sh "rm", "-rf", support_dir
		sh "rm", "-rf", nginx_dir
		
		require 'phusion_passenger/standalone/runtime_installer'
		installer = RuntimeInstaller.new(
			:source_root => SOURCE_ROOT,
			:support_dir => support_dir,
			:nginx_dir   => nginx_dir,
			:version     => @options[:nginx_version],
			:tarball     => @options[:nginx_tarball],
			:download_binaries => false)
		installer.start
		
		Dir.chdir(support_dir) do
			support_dir_name = File.basename(support_dir)
			puts "cd #{support_dir}"
			sh "tar -c . | gzip --best > ../#{support_dir_name}.tar.gz"
		end
		Dir.chdir(nginx_dir) do
			nginx_dir_name   = File.basename(nginx_dir)
			puts "cd #{nginx_dir}"
			sh "tar -c . | gzip --best > ../#{nginx_dir_name}.tar.gz"
		end
		puts "cd #{runtime_dir}"
		sh "rm", "-rf", support_dir, nginx_dir
	end

private
	def sh(*args)
		puts args.join(' ')
		if !system(*args)
			STDERR.puts "*** Cannot run command: #{args.join(' ')}"
			exit 1
		end
	end
end

end
end
