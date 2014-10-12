#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
module Config

class Command
	def self.description
		return nil
	end

	def initialize(argv)
		@argv = argv.dup
		@options = self.class.create_default_options
	end

private
	def self.create_default_options
		return {}
	end

	def parse_options
		@parser = self.class.create_option_parser(@options)
		begin
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
end

end # module Config
end # module PhusionPassenger
