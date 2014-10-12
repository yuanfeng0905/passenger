#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger

class ProgressBar
	THROBBLER = ["-", "\\", "|", "/", "-"]

	def initialize(output = STDOUT)
		@output = output
		@tty = output.tty?
		@throbbler_index = 0
	end

	def set(percentage)
		if @tty
			width = (percentage * 50).to_i
			bar   = "*" * width
			space = " " * (50 - width)
			text = sprintf("[%s%s] %s", bar, space, THROBBLER[@throbbler_index])
			@throbbler_index = (@throbbler_index + 1) % THROBBLER.size
			@output.write("#{text}\r")
			@output.flush
		else
			@output.write(".")
			@output.flush
		end
	end

	def finish
		@output.write("\n")
		@output.flush
	end
end

end # module PhusionPassenger
