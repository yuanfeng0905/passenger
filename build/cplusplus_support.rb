#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

# Rake functions for compiling/linking C++ stuff.

def compile_c(source, flags = "#{EXTRA_PRE_CFLAGS} #{PlatformInfo.portability_cflags} #{EXTRA_CXXFLAGS}")
	sh "#{CC} #{flags} -c #{source}"
end

def compile_cxx(source, flags = "#{EXTRA_PRE_CXXFLAGS} #{PlatformInfo.portability_cflags} #{EXTRA_CXXFLAGS}")
	sh "#{CXX} #{flags} -c #{source}"
end

def create_static_library(target, sources)
	# On OS X, 'ar cru' will sometimes fail with an obscure error:
	#
	#   ar: foo.a is a fat file (use libtool(1) or lipo(1) and ar(1) on it)
	#   ar: foo.a: Inappropriate file type or format
	#
	# So here we delete the ar file before creating it, which bypasses this problem.
	sh "rm -rf #{target}"
	sh "ar cru #{target} #{sources}"
	sh "ranlib #{target}"
end

def create_executable(target, sources, linkflags = "#{EXTRA_PRE_CXXFLAGS} #{EXTRA_PRE_LDFLAGS} #{PlatformInfo.portability_cflags} #{EXTRA_CXXFLAGS} #{PlatformInfo.portability_ldflags} #{EXTRA_LDFLAGS}")
	sh "#{CXX} #{sources} -o #{target} #{linkflags}"
end

def create_c_executable(target, sources, linkflags = "#{EXTRA_PRE_CFLAGS} #{EXTRA_PRE_LDFLAGS} #{PlatformInfo.portability_cflags} #{EXTRA_CXXFLAGS} #{PlatformInfo.portability_ldflags} #{EXTRA_LDFLAGS}")
	sh "#{CC} #{sources} -o #{target} #{linkflags}"
end

def create_shared_library(target, sources, flags = "#{EXTRA_PRE_CXXFLAGS} #{EXTRA_PRE_LDFLAGS} #{PlatformInfo.portability_cflags} #{EXTRA_CXXFLAGS} #{PlatformInfo.portability_ldflags} #{EXTRA_LDFLAGS}")
	if RUBY_PLATFORM =~ /darwin/
		shlib_flag = "-flat_namespace -bundle -undefined dynamic_lookup"
	else
		shlib_flag = "-shared"
	end
	sh "#{CXX} #{shlib_flag} #{sources} -fPIC -o #{target} #{flags}"
end
