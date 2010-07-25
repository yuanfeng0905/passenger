# encoding: binary
#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

require 'socket'
require 'phusion_passenger/message_channel'
require 'phusion_passenger/utils'
require 'phusion_passenger/utils/tmpdir'

module PhusionPassenger

# A convenience class for communicating with MessageServer servers,
# for example the ApplicationPool server.
class MessageClient
	include Utils
	
	# Connect to the given server. By default it connects to the current
	# generation's helper server.
	def initialize(username, password, address = "unix:#{Utils.passenger_tmpdir}/socket")
		@socket = connect_to_server(address)
		begin
			@channel = MessageChannel.new(@socket)
			
			result = @channel.read
			if result.nil?
				raise EOFError
			elsif result.size != 2 || result[0] != "version"
				raise IOError, "The message server didn't sent a valid version identifier"
			elsif result[1] != "1"
				raise IOError, "Unsupported message server protocol version #{result[1]}"
			end
			
			@channel.write_scalar(username)
			@channel.write_scalar(password)
		
			result = @channel.read
			if result.nil?
				raise EOFError
			elsif result[0] != "ok"
				raise SecurityError, result[0]
			end
		rescue Exception
			@socket.close
			raise
		end
	end
	
	def close
		@socket.close if @socket
		@channel = @socket = nil
	end
	
	def connected?
		return !!@channel
	end
	
	### ApplicationPool::Server methods ###
	
	def detach(detach_key)
		write("detach", detach_key)
		check_security_response
		result = read
		if result.nil?
			raise EOFError
		else
			return result.first == "true"
		end
	end
	
	def status
		write("inspect")
		check_security_response
		return read_scalar
	rescue
		auto_disconnect
		raise
	end
	
	def xml
		write("toXml", true)
		check_security_response
		return read_scalar
	end
	
	### BacktracesServer methods ###
	
	def backtraces
		write("backtraces")
		check_security_response
		return read_scalar
	end
	
	### Low level I/O methods ###
	
	def read
		return @channel.read
	rescue
		auto_disconnect
		raise
	end
	
	def write(*args)
		@channel.write(*args)
	rescue
		auto_disconnect
		raise
	end
	
	def write_scalar(*args)
		@channel.write_scalar(*args)
	rescue
		auto_disconnect
		raise
	end
	
	def read_scalar
		return @channel.read_scalar
	rescue
		auto_disconnect
		raise
	end
	
	def recv_io(klass = IO, negotiate = true)
		return @channel.recv_io(klass, negotiate)
	rescue
		auto_disconnect
		raise
	end
	
	def check_security_response
		begin
			result = @channel.read
		rescue
			auto_disconnect
			raise
		end
		if result.nil?
			raise EOFError
		elsif result[0] != "Passed security"
			raise SecurityError, result[0]
		end
	end

private
	def auto_disconnect
		if @channel
			@socket.close rescue nil
			@socket = @channel = nil
		end
	end
end

end # module PhusionPassenger
