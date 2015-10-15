#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion Holding B.V.
#
#  "Passenger", "Phusion Passenger" and "Union Station" are registered
#  trademarks of Phusion Holding B.V.
#
#  See LICENSE file for license information.
PhusionPassenger.require_passenger_lib 'standalone/command'

module PhusionPassenger
  module Standalone

    class VersionCommand < Command
      def run
        puts "Phusion Passenger version #{VERSION_STRING}"
        puts
        puts '"Passenger", "Phusion Passenger" and "Union Station" ' \
          'are registered trademarks of Phusion Holding B.V.'
      end
    end

  end
end
