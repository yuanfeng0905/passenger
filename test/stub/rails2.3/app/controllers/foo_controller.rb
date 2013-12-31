class FooController < ActionController::Base
	def new
		render :text => 'front page'
	end
	
	def rails_env
		render :text => RAILS_ENV
	end
	
	def backtrace
		render :text => caller.join("\n")
	end
	
	def sleep_until_exists
		File.open("#{RAILS_ROOT}/waiting_#{params[:name]}", 'w')
		while !File.exist?("#{RAILS_ROOT}/#{params[:name]}")
			sleep 0.1
		end
		render :nothing => true
	end
	
	def pid
		render :text => Process.pid
	end
	
	def allocate_memory
		$global_variable = " " * (1024 * 1024 * 125)
		render :text => 'ok'
	end
end
