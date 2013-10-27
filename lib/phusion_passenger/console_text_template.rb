#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'erb'
require 'phusion_passenger/utils/ansi_colors'

module PhusionPassenger

class ConsoleTextTemplate
	def initialize(input, options = {})
		@buffer = ''
		if input[:file]
			filename = "#{PhusionPassenger.resources_dir}/templates/#{input[:file]}.txt.erb"
			data = File.read(filename)
		else
			data = input[:text]
		end
		@template = ERB.new(Utils::AnsiColors.ansi_colorize(data),
			nil, '-', '@buffer')
		@template.filename = filename if filename
		options.each_pair do |name, value|
			self[name] = value
		end
	end
	
	def []=(name, value)
		instance_variable_set("@#{name}".to_sym, value)
		return self
	end
	
	def result
		return @template.result(binding)
	end
end

end # module PhusionPassenger
