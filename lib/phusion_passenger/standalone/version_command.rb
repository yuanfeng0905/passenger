#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.
PhusionPassenger.require_passenger_lib 'standalone/command'

module PhusionPassenger
  module Standalone

    class VersionCommand < Command
      def run
        puts "Phusion Passenger version #{VERSION_STRING}"
        puts
        puts '"Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.'
      end
    end

  end
end
