#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010 Phusion
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
			data = File.read("#{PhusionPassenger.resources_dir}/templates/#{input[:file]}.txt.erb")
		else
			data = input[:text]
		end
		@template = ERB.new(Utils::AnsiColors.ansi_colorize(data),
			nil, '-', '@buffer')
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
