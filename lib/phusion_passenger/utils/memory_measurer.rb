# encoding: binary
#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2008, 2009 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
module Utils

# Class for measuring a process's memory usage. Because querying memory usage
# can be an expensive operation, this class caches results for a short period
# of time.
class MemoryMeasurer
	MEASUREMENT_METHODS = [:proc_smaps, :proc_status, :ps]
	
	# Querying memory usage can be expensive. Here we specify how much time
	# one should wait before measuring memory usage again in order not to
	# penalize performance by too much.
	THROTTLE_RATES = {
		:proc_smaps  => 0.25,
		:proc_status => 0.1,
		:ps          => 1.0
	}
	
	attr_reader :next_measurement_time
	
	def initialize
		@last_measurement_result = nil
		@next_measurement_time = Time.now
	end
	
	# Measures the memory usage of a process. Returns the memory usage
	# in MB, or nil if it cannot be measured.
	#
	# The memory usage measured is either the private dirty RSS or the RSS,
	# depending on the platform.
	def measure(pid = Process.pid, use_cache = true)
		if use_cache && current_time < @next_measurement_time
			return @last_measurement_result
		elsif @measurement_method
			result = send(:"measure_with_#{@measurement_method}", pid)
			@last_measurement_result = result
			@next_measurement_time   = current_time + THROTTLE_RATES[@measurement_method]
			return result
		else
			MEASUREMENT_METHODS.each do |method|
				result = send(:"measure_with_#{method}", pid)
				if result
					@measurement_method      = method
					@last_measurement_result = result
					@next_measurement_time   = current_time + THROTTLE_RATES[method]
					return result
				end
			end
			@last_measurement_result = nil
			@next_measurement_time   = current_time + THROTTLE_RATES[MEASUREMENT_METHODS.last]
			return nil
		end
	end

private
	def current_time
		return Time.now
	end
	
	# Sigh... there's no portable way to query the current process's memory usage. :-(
	# So we'll use an army of platform-specific functions to query/estimate the
	# memory usage.
	
	# OS X NOTE: on Snow Leopard, 'ps -o vsz' seems to be broken: it reports VM sizes
	# of about 2 GB even for the smallest processes like 'bash'. Therefore we don't
	# measure the VM size and measure the RSS instead.
	
	# Get a process's memory usage by querying /proc/(PID)/smaps.
	# The private dirty RSS of the current process will be queried. This is
	# by far the most accurate way of measuring memory usage.
	#
	# As far as I know, Linux is the only operating system that supports this.
	#
	# Returns the memory usage in MB, or nil if the information could not be
	# queried.
	def measure_with_proc_smaps(pid)
		total = 0
		data = File.open("/proc/#{pid}/smaps", "rb") do |f|
			f.read
		end
		data.split("\n").each do |line|
			line =~ /^(Private)_Dirty: +(\d+)/
			if $2
				total += $2.to_i
			end
		end
		if total == 0
			return nil
		else
			# 'total' is in KB.
			return total / 1024.0
		end
	rescue SystemCallError, IOError
		return nil
	end
	
	# Get a process's RSS by querying /proc/(PID)/status.
	#
	# Some Linux systems don't have /proc/(PID)/smaps or don't have a usable
	# /proc/(PID)/smaps, for whatever reason. In that case, this method may be
	# used instead to estimate the memory usage.
	#
	# FreeBSD with /proc mounted with linprocfs also supports /proc/(PID)/status.
	# It's recommended that the system administrator mounts /proc with linprocfs
	# instead of procfs, because FreeBSD provides no other lightweight way to
	# query memory usage. Using 'ps' (like measure_with_ps) is much, MUCH slower
	# and should generally be avoided.
	#
	# Returns the RSS in MB, or nil if the information could not be queried.
	def measure_with_proc_status(pid)
		data = File.open("/proc/#{pid}/status", "rb") do |f|
			f.read
		end
		data =~ /^VmRSS: *(.*) kB$/
		if $1
			# $1 is in KB.
			return $1.to_i / 1024.0
		else
			return nil
		end
	rescue SystemCallError, IOError
		return nil
	end
	
	# Get a process's RSS by querying the 'ps' command. This is the most portable
	# way to obtain the RSS, but is also the slowest, than thus should be avoided
	# whenever possible.
	#
	# Returns the RSS in MB, or nil if the information could not be queried.
	def measure_with_ps(pid)
		output = `ps -p#{pid} -o rss`
		if output.nil?
			return nil
		else
			output.strip!
			vm_size = output.split("\n")[1]
			if vm_size
				vm_size.strip!
				return vm_size.to_i / 1024.0   # vm_size is in KB.
			else
				return nil
			end
		end
	end
end

end
end