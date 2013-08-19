#!/usr/bin/env ruby
FILES = Dir[
	"bin/*",
	"build/*.rb",
	"ext/{apache2,common,nginx,ruby}/**/*.{c,cpp,h,hpp,erb}",
	"helper-scripts/**/*",
	"lib/**/*.rb",
	"lib/**/*.py",
]
LICENSE_START_SIGNATURE = '"Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.'
NEW_LICENSE = "See LICENSE file for license information."

def change_license!(filename)
	result = false
	contents = File.read(filename)
	if contents =~ /\A((#.*?\n)+)/m
		header  = $1
		license_start = contents.index(LICENSE_START_SIGNATURE)
		if license_start
			license_start += LICENSE_START_SIGNATURE.size
			license_end = header.size
			contents[license_start .. license_end - 1] = "\n#\n#  #{NEW_LICENSE}\n"
			result = true
		end
	elsif contents =~ %r(\A(/\*.+?\*/))m
		header = $1
		license_start = contents.index(LICENSE_START_SIGNATURE)
		if license_start
			license_start += LICENSE_START_SIGNATURE.size
			license_end = header.size
			contents[license_start .. license_end - 1] = "\n *\n *  #{NEW_LICENSE}\n */"
			result = true
		end
	end
	File.open(filename, 'w') do |f|
		f.write(contents)
	end
	return result
rescue EOFError
end

FILES.each do |filename|
	next if File.directory?(filename)
	printf("%-65s", filename)
	STDOUT.flush
	if change_license!(filename)
		puts "  ok!"
	else
		puts "  skipped"
	end
end