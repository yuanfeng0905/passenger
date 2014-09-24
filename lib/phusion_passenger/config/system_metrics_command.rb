#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'config/command'

module PhusionPassenger
module Config

class SystemMetricsCommand < Command
	def run
		exec("#{PhusionPassenger.agents_dir}/PassengerAgent",
			"system-metrics", *@argv)
	end
end

end # module Config
end # module PhusionPassenger
