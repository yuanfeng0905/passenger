# encoding: binary
#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'phusion_passenger/abstract_request_handler'
require 'phusion_passenger/classic_rails/cgi_fixed'
module PhusionPassenger
module ClassicRails

# A request handler for Ruby on Rails applications.
class RequestHandler < AbstractRequestHandler
	def initialize(owner_pipe, options = {})
		super(owner_pipe, options)
	end

protected
	# Overrided method.
	def process_request(headers, input, output, status_line_desired)
		cgi = CGIFixed.new(headers, input, output)
		::Dispatcher.dispatch(cgi,
			::ActionController::CgiRequest::DEFAULT_SESSION_OPTIONS,
			cgi.stdoutput)
	end
end

end # module ClassicRails
end # module PhusionPassenger
