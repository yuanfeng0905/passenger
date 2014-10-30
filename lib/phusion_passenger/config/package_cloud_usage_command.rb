#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'optparse'
require 'tmpdir'
require 'fileutils'
PhusionPassenger.require_passenger_lib 'ruby_core_enhancements'
PhusionPassenger.require_passenger_lib 'config/command'
PhusionPassenger.require_passenger_lib 'utils/shellwords'

module PhusionPassenger
module Config

class PackageCloudUsageCommand < Command
	def run
		parse_options
		data_dir = "#{PhusionPassenger.home_dir}/#{USER_NAMESPACE_DIRNAME}/usage_data"
		if File.exist?(data_dir)
			create_archive(data_dir)
			clear_data_dir(data_dir)
		else
			Dir.mktmpdir do |tmpdir|
				create_archive(tmpdir)
			end
		end
	end

private
	def self.create_option_parser(options)
		OptionParser.new do |opts|
			nl = "\n" + ' ' * 37
			opts.banner = "Usage: passenger-config package-cloud-usage <OUTPUT.tar.gz> [OPTIONS]\n"
			opts.separator ""
			opts.separator "  Package recorded cloud data usage points into an archive file, so that it can"
			opts.separator "  be sent to Phusion later."
			opts.separator ""

			opts.separator "Options:"
			opts.on("-h", "--help", "Show this help") do
				options[:help] = true
			end
		end
	end

	def parse_options
		super
		if @argv.empty?
			abort "Please specify an output filename. See --help for more information."
		elsif @argv.size > 1
			abort "Please specify only a single output filename. See --help for more information."
		else
			@output_filename = File.absolute_path_no_resolve(@argv[0])
		end
	end

	def create_archive(data_dir)
		Dir.mktmpdir do |tmpdir|
			Dir.mkdir("#{tmpdir}/usage_data")
			FileUtils.cp(Dir["#{data_dir}/*"], "#{tmpdir}/usage_data")
			File.open("#{tmpdir}/hostname", "w") do |f|
				f.write(detect_hostname)
			end
			File.open("#{tmpdir}/machine-properties.json", "w") do |f|
				f.write(collect_machine_properties)
			end
			Dir.chdir(tmpdir) do
				if !system("tar", "-czf", @output_filename, ".")
					abort "Unable to create package: tar failed."
				end
			end
		end
	end

	def clear_data_dir(data_dir)
		files = Dir["#{data_dir}/*"]
		if !files.empty?
			FileUtils.rm(files)
		end
	end

	def detect_hostname
		hostname = `hostname`.strip
		if hostname.empty?
			abort "Unable to query the current machine's host name."
		end
	end

	def collect_machine_properties
		agent_exe = "#{PhusionPassenger.agents_dir}/PassengerHelperAgent"
		command = "#{agent_exe} send-cloud-usage " +
			"--passenger-root #{Shellwords.escape PhusionPassenger.source_root} " +
			"--dump-machine-properties"
		result = `#{command}`.strip
		if result.empty?
			abort "The command '#{command}' failed."
		end
		return result
	end
end

end # module Config
end # module PhusionPassenger
