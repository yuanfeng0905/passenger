#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

### Node.js components tests ###

desc "Run unit tests for the Node.js libraries"
task 'test:node' do
	require 'shellwords'
	command = "cd test && env NODE_PATH=#{PhusionPassenger.node_libdir} " +
		"../node_modules/.bin/mocha -R spec node/*_spec.js"
	if grep = string_option('G')
		command << " -g #{Shellwords.escape(grep)}"
	end
	sh(command)
end
