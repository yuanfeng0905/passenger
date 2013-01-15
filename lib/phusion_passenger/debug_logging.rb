#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger

module DebugLogging
	# We don't refer to STDERR directly because STDERR's reference might
	# change during runtime.
	@@log_level = 0
	@@log_device = nil
	@@log_filename = nil
	@@stderr_evaluator = lambda { STDERR }
	
	def self.included(klass)
		klass.class_eval do
			private :debug
			private :trace
		end
	end
	
	def self.log_level=(level)
		@@log_level = level
	end
	
	def self.log_file=(filename)
		if filename && filename.empty?
			@@log_filename = nil
		else
			@@log_filename = filename
		end
		@@log_device.close if @@log_device && !@@log_device.closed?
		@@log_device = nil
	end
	
	def self._log_device
		return @@log_device
	end
	
	def self.stderr_evaluator=(block)
		if block
			@@stderr_evaluator = block
		else
			@@stderr_evaluator = lambda { STDERR }
		end
	end
	
	def error(message)
		trace(-1, message, 1)
	end
	module_function :error
	
	def warn(message)
		trace(0, message, 1)
	end
	module_function :warn
	
	def debug(message)
		trace(1, message, 1)
	end
	module_function :debug
	
	def trace(level, message, nesting_level = 0)
		if @@log_level >= level
			if @@log_filename
				if !@@log_device || @@log_device.closed?
					@@log_device = File.open(@@log_filename, "a")
				end
				output = @@log_device
			else
				output = @@stderr_evaluator.call
			end
			location = caller[nesting_level].sub(/.*phusion_passenger\//, '')
			location.sub!(/(.*):.*/, '\1')
			now = Time.now
			time_str = now.strftime("%Y-%m-%d %H:%M:%S.")
			time_str << sprintf("%04d", now.usec / 100)
			if thread_name = Thread.current[:name]
				thread_name = "(#{thread_name})"
			end
			output.write("[ pid=#{$$} thr=0x#{Thread.current.object_id.to_s(16)}#{thread_name} file=#{location} time=#{time_str} ]: #{message}\n")
			output.flush
		end
	end
	module_function :trace
end

end
