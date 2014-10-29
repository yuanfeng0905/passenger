#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'config/command'

module PhusionPassenger
module Config

class SendCloudUsageCommand < Command
	def run
		exec("#{PhusionPassenger.agents_dir}/PassengerHelperAgent",
			"send-cloud-usage", "--passenger-root", PhusionPassenger.source_root,
			*@argv)
	end
end

end # module Config
end # module PhusionPassenger
