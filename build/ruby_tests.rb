#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

### Ruby components tests ###

dependencies = [NATIVE_SUPPORT_TARGET, AGENT_TARGET,
  "#{TEST_OUTPUT_DIR}allocate_memory"].compact
desc "Run unit tests for the Ruby libraries"
task 'test:ruby' => dependencies do
  if maybe_grep = string_option('E')
    require 'shellwords'
    maybe_grep = "-e #{Shellwords.escape(maybe_grep)}"
  end
  command = "rspec -c -f s --tty -P 'dont-autoload-anything' #{maybe_grep} ruby/*_spec.rb ruby/*/*_spec.rb"
  sh "cd test && exec bundle exec #{command}"
end
