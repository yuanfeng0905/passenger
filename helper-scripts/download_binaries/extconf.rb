#!/usr/bin/env ruby
#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

# This script is called during 'gem install'. Its role is to download
# Phusion Passenger binaries.

# Create a dummy Makefile to prevent 'gem install' from borking out.
File.open("Makefile", "w") do |f|
	f.puts "all:"
	f.puts "	true"
	f.puts "install:"
	f.puts "	true"
end

# Don't do anything on Windows. We don't support Windows but exiting now
# will at least prevent the gem from being not installable on Windows.
exit if RUBY_PLATFORM =~ /mswin/i || RUBY_PLATFORM =~ /win32/i

source_root = File.expand_path("../..", File.dirname(__FILE__))
$LOAD_PATH.unshift("#{source_root}/lib")
require 'phusion_passenger'
PhusionPassenger.locate_directories
require 'fileutils'

if PhusionPassenger.natively_packaged?
	puts "Binary downloading is only available when originally packaged. Stopping."
	exit 0
end
if !PhusionPassenger.installed_from_release_package?
	puts "This Phusion Passenger is not installed from an official release package. Stopping."
	exit 0
end

# Create download directory and do some preparation
require 'phusion_passenger/platform_info'
require 'phusion_passenger/platform_info/binary_compatibility'
cxx_compat_id = PhusionPassenger::PlatformInfo.cxx_binary_compatibility_id
ruby_compat_id = PhusionPassenger::PlatformInfo.ruby_extension_binary_compatibility_id

FileUtils.mkdir_p(PhusionPassenger.download_cache_dir)
Dir.chdir(PhusionPassenger.download_cache_dir)

# Initiate downloads
def download(name)
	if !File.exist?(name)
		url = "#{PhusionPassenger::BINARIES_URL_ROOT}/#{PhusionPassenger::VERSION_STRING}/#{name}"
		cert = PhusionPassenger.binaries_ca_cert_path
		puts "Attempting to download #{url} into #{Dir.pwd}"
		File.unlink("#{name}.tmp") rescue nil
		if PhusionPassenger::PlatformInfo.find_command("wget")
			result = system("wget", "--tries=3", "-O", "#{name}.tmp", "--ca-certificate=#{cert}", url)
		else
			result = system("curl", url, "-f", "-L", "-o", "#{name}.tmp", "--cacert", cert)
		end
		if result
			File.rename("#{name}.tmp", name)
		else
			File.unlink("#{name}.tmp") rescue nil
		end
	else
		puts "#{Dir.pwd}/#{name} already exists"
	end
end

download "support-#{cxx_compat_id}.tar.gz"
download "nginx-#{PhusionPassenger::PREFERRED_NGINX_VERSION}-#{cxx_compat_id}.tar.gz"
download "rubyext-#{ruby_compat_id}.tar.gz"
