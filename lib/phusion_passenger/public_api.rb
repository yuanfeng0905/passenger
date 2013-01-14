#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
class << self
	@@event_starting_worker_process = []
	@@event_stopping_worker_process = []
	@@event_credentials = []
	@@event_after_installing_signal_handlers = []
	@@event_oob_work = []
	
	def on_event(name, &block)
		callback_list_for_event(name) << block
	end
	
	def call_event(name, *args)
		callback_list_for_event(name).each do |callback|
			callback.call(*args)
		end
	end
	
	def install_framework_extensions!(*args)
		require 'rails/version' if defined?(::Rails) && !defined?(::Rails::VERSION)
		if defined?(::Rails) && ::Rails::VERSION::MAJOR == 3
			require 'phusion_passenger/rails3_extensions/init'
			Rails3Extensions.init!(PhusionPassenger::App.options, *args)
		end
	end
	
	def benchmark(env = nil, title = "Benchmarking")
		log = lookup_analytics_log(env)
		if log
			log.measure("BENCHMARK: #{title}") do
				yield
			end
		else
			yield
		end
	end
	
	def log_cache_hit(env, name)
		log = lookup_analytics_log(env)
		if log
			log.message("Cache hit: #{name}")
			return true
		else
			return false
		end
	end
	
	def log_cache_miss(env, name, generation_time = nil)
		log = lookup_analytics_log(env)
		if log
			if generation_time
				log.message("Cache miss (#{generation_time.to_i}): #{name}")
			else
				log.message("Cache miss: #{name}")
			end
			return true
		else
			return false
		end
	end

private
	def callback_list_for_event(name)
		return case name
		when :starting_worker_process
			@@event_starting_worker_process
		when :stopping_worker_process
			@@event_stopping_worker_process
		when :credentials
			@@event_credentials
		when :after_installing_signal_handlers
			@@event_after_installing_signal_handlers
		when :oob_work
			@@event_oob_work
		else
			raise ArgumentError, "Unknown event name '#{name}'"
		end
	end
	
	def lookup_analytics_log(env)
		if env
			return env[PASSENGER_ANALYTICS_WEB_LOG]
		else
			return Thread.current[PASSENGER_ANALYTICS_WEB_LOG]
		end
	end

end
end # module PhusionPassenger
