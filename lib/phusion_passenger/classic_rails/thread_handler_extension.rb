# encoding: binary
#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'classic_rails/cgi_fixed'
module PhusionPassenger
module ClassicRails

module ThreadHandlerExtension
	def process_request(env, connection, socket_wrapper, full_http_response)
		cgi = CGIFixed.new(env, connection, connection)
		::Dispatcher.dispatch(cgi,
			::ActionController::CgiRequest::DEFAULT_SESSION_OPTIONS,
			cgi.stdoutput)
		return false # Socket hijacking not supported.
	end
end

end # module ClassicRails
end # module PhusionPassenger
