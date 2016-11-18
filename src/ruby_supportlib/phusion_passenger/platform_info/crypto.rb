#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2016 Phusion Holding B.V.
#
#  "Passenger", "Phusion Passenger" and "Union Station" are registered
#  trademarks of Phusion Holding B.V.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'platform_info'
PhusionPassenger.require_passenger_lib 'platform_info/operating_system'

module PhusionPassenger

  module PlatformInfo
    def self.crypto_libs
      if os_name_simple == "macosx"
        return ' -framework CoreFoundation -framework Security'
      else
        return ' -lcrypto'
      end
    end
    memoize :crypto_libs

    def self.crypto_extra_cflags
      if os_name_simple == "macosx"
        return ' -Wno-deprecated-declarations'
      else
        return ''
      end
    end
    memoize :crypto_extra_cflags

  end

end # module PhusionPassenger
