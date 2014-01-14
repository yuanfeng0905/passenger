#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

### Ruby components tests ###

dependencies = [NATIVE_SUPPORT_TARGET, AGENT_OUTPUT_DIR + 'PassengerLoggingAgent', 'test/support/allocate_memory'].compact
desc "Run unit tests for the Ruby libraries"
task 'test:ruby' => dependencies do
	if PlatformInfo.rspec.nil?
		abort "RSpec is not installed for Ruby interpreter '#{PlatformInfo.ruby_command}'. Please install it."
	else
		if maybe_grep = string_option('E')
			require 'shellwords'
			maybe_grep = "-e #{Shellwords.escape(maybe_grep)}"
		end
		command = "#{PlatformInfo.rspec} -c -f s -P 'dont-autoload-anything' #{maybe_grep} ruby/*_spec.rb ruby/*/*_spec.rb"
		sh "cd test && exec #{command}"
	end
end

dependencies = [NATIVE_SUPPORT_TARGET].compact
desc "Run coverage tests for the Ruby libraries"
task 'test:rcov' => dependencies do
	if PlatformInfo.rspec.nil?
		abort "RSpec is not installed for Ruby interpreter '#{PlatformInfo.ruby_command}'. Please install it."
	else
		Dir.chdir("test") do
			sh "rcov", "--exclude",
				"lib\/spec,\/spec$,_spec\.rb$,support\/,platform_info,integration_tests",
				PlatformInfo.rspec, "--", "-c", "-f", "s",
				*Dir["ruby/*.rb", "ruby/*/*.rb", "integration_tests.rb"]
		end
	end
end
