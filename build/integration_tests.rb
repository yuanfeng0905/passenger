#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

### Integration tests ###

desc "Run all integration tests"
task 'test:integration' => ['test:integration:apache2', 'test:integration:nginx'] do
end

dependencies = [:apache2, NATIVE_SUPPORT_TARGET, 'test/support/allocate_memory'].compact
desc "Run Apache 2 integration tests"
task 'test:integration:apache2' => dependencies do
	if PlatformInfo.rspec.nil?
		abort "RSpec is not installed for Ruby interpreter '#{PlatformInfo.ruby_command}'. Please install it."
	else
		command = "#{PlatformInfo.rspec} -c -f s integration_tests/apache2_tests.rb"
		if boolean_option('SUDO')
			command = "#{PlatformInfo.ruby_sudo_command} -E #{command}"
		end
		if grep = string_option('E')
			require 'shellwords'
			command << " -e #{Shellwords.escape(grep)}"
		end
		sh "cd test && #{command}"
	end
end

dependencies = [:nginx, NATIVE_SUPPORT_TARGET].compact
desc "Run Nginx integration tests"
task 'test:integration:nginx' => dependencies do
	if PlatformInfo.rspec.nil?
		abort "RSpec is not installed for Ruby interpreter '#{PlatformInfo.ruby_command}'. Please install it."
	else
		command = "#{PlatformInfo.rspec} -c -f s integration_tests/nginx_tests.rb"
		if boolean_option('SUDO')
			command = "#{PlatformInfo.ruby_sudo_command} -E #{command}"
		end
		if grep = string_option('E')
			require 'shellwords'
			command << " -e #{Shellwords.escape(grep)}"
		end
		sh "cd test && #{command}"
	end
end

dependencies = [:nginx, NATIVE_SUPPORT_TARGET].compact
desc "Run Passenger Standalone integration tests"
task 'test:integration:standalone' => dependencies do
	if PlatformInfo.rspec.nil?
		abort "RSpec is not installed for Ruby interpreter '#{PlatformInfo.ruby_command}'. Please install it."
	else
		Dir.chdir("test") do
			#ruby "#{PlatformInfo.rspec} -c -f s integration_tests/standalone_tests.rb"
		end
	end
end

desc "Run native packaging tests"
task 'test:integration:native_packaging' do
	if PlatformInfo.rspec.nil?
		abort "RSpec is not installed for Ruby interpreter '#{PlatformInfo.ruby_command}'. Please install it."
	else
		command = "#{PlatformInfo.rspec} -c -f s integration_tests/native_packaging_spec.rb"
		if boolean_option('SUDO')
			command = "#{PlatformInfo.ruby_sudo_command} -E #{command}"
		end
		sh "cd test && #{command}"
	end
end

dependencies = [:apache2, NATIVE_SUPPORT_TARGET].compact
desc "Run the 'apache2' integration test infinitely, and abort if/when it fails"
task 'test:restart' => dependencies do
	require 'shellwords'
	color_code_start = "\e[33m\e[44m\e[1m"
	color_code_end = "\e[0m"
	i = 1
	while true do
		puts "#{color_code_start}Test run #{i} (press Ctrl-C multiple times to abort)#{color_code_end}"
		command = "cd test && rspec -c -f s integration_tests/apache2_tests.rb"
		if grep = string_option('E')
			command << " -e #{Shellwords.escape(grep)}"
		end
		sh(command)
		i += 1
	end
end
