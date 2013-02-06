#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

desc "Build Nginx support files"
task :nginx => [
	:nginx_without_native_support,
	NATIVE_SUPPORT_TARGET
].compact

task :nginx_without_native_support => [
	AGENT_OUTPUT_DIR + 'PassengerHelperAgent',
	AGENT_OUTPUT_DIR + 'PassengerWatchdog',
	AGENT_OUTPUT_DIR + 'PassengerLoggingAgent',
	AGENT_OUTPUT_DIR + 'SpawnPreparer',
	COMMON_LIBRARY.only(*NGINX_LIBS_SELECTOR).link_objects
].flatten

task :clean => 'nginx:clean'
desc "Clean all compiled Nginx files"
task 'nginx:clean' => 'common:clean' do
	# Nothing to clean at this time.
end
