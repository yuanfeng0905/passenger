#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

if !PlatformInfo.passenger_needs_ruby_dev_header?
	NATIVE_SUPPORT_TARGET = nil

	task :native_support do
		# Do nothing.
	end

	task 'native_support:clean' do
		# Do nothing.
	end
else
	output_dir  = RUBY_EXTENSION_OUTPUT_DIR
	output_name = "passenger_native_support.#{LIBEXT}"
	source_dir  = "ext/ruby"
	NATIVE_SUPPORT_TARGET = "#{output_dir}/#{output_name}"

	task :native_support => "#{output_dir}/#{output_name}"
	task :clean => 'native_support:clean'

	dependencies = [
		"#{output_dir}/Makefile",
		"#{source_dir}/passenger_native_support.c"
	]
	file("#{output_dir}/#{output_name}" => dependencies) do
		sh "mkdir -p '#{output_dir}'" if !File.exist?(output_dir)
		sh "cd '#{output_dir}' && make"
	end

	file "#{output_dir}/Makefile" => "#{source_dir}/extconf.rb" do
		sh "mkdir -p '#{output_dir}'" if !File.exist?(output_dir)
		extconf_rb = File.expand_path("#{source_dir}/extconf.rb")
		sh "cd '#{output_dir}' && #{PlatformInfo.ruby_command} '#{extconf_rb}'"
	end

	task 'native_support:clean' do
		sh "echo rm -rf #{output_dir}"
	end
end
