#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger

module Utils
protected
	@@passenger_tmpdir = nil
	
	def passenger_tmpdir(create = true)
		PhusionPassenger::Utils.passenger_tmpdir(create)
	end
	
	# Returns the directory in which to store Phusion Passenger-specific
	# temporary files. If +create+ is true, then this method creates the
	# directory if it doesn't exist.
	def self.passenger_tmpdir(create = true)
		dir = @@passenger_tmpdir
		if dir.nil? || dir.empty?
			tmpdir = "/tmp"
			["PASSENGER_TEMP_DIR", "PASSENGER_TMPDIR"].each do |name|
				if ENV.has_key?(name) && !ENV[name].empty?
					tmpdir = ENV[name]
					break
				end
			end
			dir = "#{tmpdir}/passenger.1.0.#{Process.pid}"
			dir.gsub!(%r{//+}, '/')
			@@passenger_tmpdir = dir
		end
		if create && !File.exist?(dir)
			# This is a very minimal implementation of the subdirectory
			# creation logic in ServerInstanceDir.h. This implementation
			# is only meant to make the unit tests pass. For production
			# systems one should pre-create the temp directory with
			# ServerInstanceDir.h.
			system("mkdir", "-p", "-m", "u=rwxs,g=rwx,o=rwx", dir)
			system("mkdir", "-p", "-m", "u=rwxs,g=rwx,o=rwx", "#{dir}/generation-0")
			system("mkdir", "-p", "-m", "u=rwxs,g=rwx,o=rwx", "#{dir}/backends")
		end
		return dir
	end
	
	def self.passenger_tmpdir=(dir)
		@@passenger_tmpdir = dir
	end
end

end
