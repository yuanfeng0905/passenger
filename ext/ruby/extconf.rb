#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2013 Phusion
#
#  See LICENSE file for license information.
require 'mkmf'
$LIBS = ""
$CFLAGS << " -g"
$LIBS << " -lpthread"

if RUBY_PLATFORM =~ /solaris/
	have_library('xnet')
	$CFLAGS << " -D_XPG4_2"
	$CFLAGS << " -D__EXTENSIONS__"
	if RUBY_PLATFORM =~ /solaris2.9/
		$CFLAGS << " -D__SOLARIS9__"
	end
end

have_header('alloca.h')
have_header('ruby/version.h')
have_header('ruby/io.h')
have_var('ruby_version')
have_func('rb_thread_io_blocking_region')

with_cflags($CFLAGS) do
	create_makefile('passenger_native_support')
	if RUBY_PLATFORM =~ /solaris/
		# Fix syntax error in Solaris /usr/ccs/bin/make.
		# https://code.google.com/p/phusion-passenger/issues/detail?id=999
		makefile = File.read("Makefile")
		makefile.sub!(/^ECHO = .*/, "ECHO = echo")
		File.open("Makefile", "w") do |f|
			f.write(makefile)
		end
	end
end
