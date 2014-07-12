#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'message_channel'

module PhusionPassenger
module UnionStation

class Connection
	attr_reader :mutex
	attr_accessor :channel

	def initialize(io)
		@mutex = Mutex.new
		@refcount = 1
		@channel = MessageChannel.new(io) if io
	end

	def connected?
		return !!@channel
	end

	def disconnect
		@channel.close if @channel
		@channel = nil
	end

	def ref
		@refcount += 1
	end

	def unref
		@refcount -= 1
		if @refcount == 0
			disconnect
		end
	end

	def synchronize
		@mutex.synchronize do
			yield
		end
	end
end

end # module UnionStation
end # module PhusionPassenger
