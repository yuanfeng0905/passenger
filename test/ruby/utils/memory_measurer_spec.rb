require File.expand_path(File.dirname(__FILE__) + '/../spec_helper')
require 'phusion_passenger/utils/memory_measurer'

module PhusionPassenger

describe Utils::MemoryMeasurer do
	before :each do
		@measurer = Utils::MemoryMeasurer.new
	end
	
	after :each do
		shutdown_process(@pid) if @pid
		shutdown_process(@pid2) if @pid2
	end
	
	# Spawn a process whose only job is to allocate +size+ amount of memory, in MB.
	# Yields the given block and passes the PID to the block.
	def spawn_process(size)
		exe = File.dirname(__FILE__) + "/../../support/allocate_memory"
		pid = IO.popen("#{exe} #{size}").pid
		# Give it some time to start.
		sleep 0.3
		return pid
	end
	
	def shutdown_process(pid = @pid)
		Process.kill('SIGKILL', pid)
		Process.waitpid(pid) rescue nil
	end
	
	specify "#get_memory_usage_by_using_smaps works" do
		@pid = spawn_process(20)
		size = @measurer.send(:measure_with_proc_smaps, @pid)
		# Only Linux supports querying with /proc/<PID>/smaps
		# at this moment.
		if RUBY_PLATFORM =~ /linux/
			(20..26).should include(size)
		else
			size.should be_nil
		end
	end
	
	specify "#get_vm_size_by_using_status works" do
		@pid = spawn_process(20)
		size = @measurer.send(:measure_with_proc_status, @pid)
		case RUBY_PLATFORM
		when /linux/
			# Linux supports this.
			(20..26).should include(size)
		when /freebsd/
			# FreeBSD only supports it if linprocfs is mounted.
			if `df /proc 2>/dev/null` =~ /linprocfs/
				(20..26).should include(size)
			else
				size.should be_nil
			end
		else
			size.should be_nil
		end
	end
	
	specify "#measure_with_ps works" do
		@pid = spawn_process(20)
		size = @measurer.send(:measure_with_ps, @pid)
		if RUBY_PLATFORM =~ /darwin/
			(20..26).should include(size)
		else
			(20..26).should include(size)
		end
	end
	
	specify "#measure" do
		@pid = spawn_process(20)
		size = @measurer.measure(@pid)
		(20..26).should include(size)
	end
	
	specify "#measure caches its last result for a short period of time" do
		# This test depends on timing. If it fails then shutdown
		# all busy processes and run this test again.
		@pid  = spawn_process(20)
		@pid2 = spawn_process(50)
		size1 = @measurer.measure(@pid)
		size2 = @measurer.measure(@pid2)
		size1.should == size2
		
		sleep_time = @measurer.next_measurement_time - Time.now
		sleep(sleep_time) if sleep_time >= 0
		size2 = @measurer.measure(@pid2)
		size1.should_not == size2
	end
end

end # module PhusionPassenger