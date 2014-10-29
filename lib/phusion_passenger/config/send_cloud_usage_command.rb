#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'config/command'
PhusionPassenger.require_passenger_lib 'constants'

module PhusionPassenger
module Config

class SendCloudUsageCommand < Command
	def run
		exec("#{PhusionPassenger.support_binaries_dir}/#{AGENT_EXE}",
			"send-cloud-usage", "--passenger-root", PhusionPassenger.install_spec,
			*@argv)
	end
end

end # module Config
end # module PhusionPassenger
