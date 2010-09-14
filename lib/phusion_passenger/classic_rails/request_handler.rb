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
	CONTENT_LENGTH      = 'CONTENT_LENGTH'      # :nodoc:
	HTTP_CONTENT_LENGTH = 'HTTP_CONTENT_LENGTH' # :nodoc:
	
	NINJA_PATCHING_LOCK = Mutex.new             # :nodoc:
	@@ninja_patched_action_controller = false
	
	def initialize(owner_pipe, options = {})
		super(owner_pipe, options)
		NINJA_PATCHING_LOCK.synchronize do
			ninja_patch_action_controller
		end
	end

protected
	# Overrided method.
	def process_request(headers, input, output, status_line_desired)
		# FIXME: how to print status line?
		headers[CONTENT_LENGTH] = headers[HTTP_CONTENT_LENGTH]
		cgi = CGIFixed.new(headers, input, output)
		::Dispatcher.dispatch(cgi,
			::ActionController::CgiRequest::DEFAULT_SESSION_OPTIONS,
			cgi.stdoutput)
	end
	
private
	def ninja_patch_action_controller
		if !@@ninja_patched_action_controller && defined?(::ActionController::Base) \
		&& ::ActionController::Base.private_method_defined?(:perform_action)
			@@ninja_patched_action_controller = true
			::ActionController::Base.class_eval do
				alias passenger_orig_perform_action perform_action
				
				def perform_action(*whatever)
					headers[X_POWERED_BY] = @passenger_header
					passenger_orig_perform_action(*whatever)
				end
				private :perform_action
			end
		end
	end
end

end # module ClassicRails
end # module PhusionPassenger
