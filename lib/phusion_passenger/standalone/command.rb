#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
module Standalone

class Command
	def initialize(argv)
		@argv = argv.dup
		@options = self.class.create_default_options
	end

private
	def self.create_default_options
		return {}
	end

	def parse_options
		load_and_merge_default_options(@options)
		@parser = self.class.create_option_parser(@options)
		begin
			@original_argv = @argv.dup
			@parser.parse!(@argv)
		rescue OptionParser::ParseError => e
			STDERR.puts "*** ERROR: #{e}"
			abort @parser.to_s
		end
		if @options[:help]
			puts @parser
			exit
		end
	end

	def load_and_merge_default_options(options)
		path = ConfigUtils.global_config_file_path
		if File.exist?(path)
			begin
				global_options = ConfigUtils.load_config_file(path)
			rescue ConfigUtils::ConfigLoadError => e
				STDERR.puts "*** Warning: #{e.message}"
				return
			end
			@options.merge!(global_options)
		end
	end
end

end # module STandalone
end # module PhusionPassenger
