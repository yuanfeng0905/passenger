# encoding: binary
#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2012 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'thread'

module PhusionPassenger
module Rack

class OutOfBandGc
  def initialize(app, frequency, logger = nil)
    @app = app
    @frequency = frequency
    @request_count = 0
    @mutex = Mutex.new
    
    ::PhusionPassenger.on_event(:oob_work) do
      t0 = Time.now
      GC.start
      logger.info "Out Of Band GC finished in #{Time.now - t0} sec" if logger
    end
  end

  def call(env)
    status, headers, body = @app.call(env)

    @mutex.synchronize do
      @request_count += 1
      if @request_count == @frequency
        @request_count = 0
        headers['X-Passenger-Request-OOB-Work'] = 'true'
      end
    end
    
    [status, headers, body]
  end
end

end # module Rack
end # module PhusionPassenger

