#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'optparse'
require 'tmpdir'
PhusionPassenger.require_passenger_lib 'config/command'

module PhusionPassenger
module Config

class PackageCloudUsageCommand < Command
	def run
		parse_options
		data_dir = File.expand_path("~/#{USER_NAMESPACE_DIRNAME}/usage_data")
		if File.exist?(data_dir)
			create_archive(data_dir)
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
			@output_filename = File.expand_path(@argv[0])
		end
	end

	def create_archive(data_dir)
		hostname = `hostname`.strip
		if hostname.nil?
			abort "Unable to query the current machine's host name."
		end

		Dir.mktmpdir do |tmpdir|
			Dir.mkdir("#{tmpdir}/usage_data")
			FileUtils.cp(Dir["#{data_dir}/*"], "#{tmpdir}/usage_data")
			File.open("#{tmpdir}/hostname", "w") do |f|
				f.write(hostname)
			end
			Dir.chdir(tmpdir) do
				system("tar", "-czf", @output_filename, ".")
			end
		end
	end
end

end # module Config
end # module PhusionPassenger
