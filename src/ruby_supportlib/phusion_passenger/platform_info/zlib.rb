#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'platform_info'

module PhusionPassenger

  module PlatformInfo
    def self.zlib_flags
      return nil
    end

    def self.zlib_libs
      return '-lz'
    end
  end

end # module PhusionPassenger
