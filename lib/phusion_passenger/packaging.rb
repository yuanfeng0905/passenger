#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger

module Packaging
	# A list of HTML files that are generated with Asciidoc.
	ASCII_DOCS = [
		'doc/Users guide Apache.html',
		'doc/Users guide Nginx.html',
		'doc/Users guide Standalone.html',
		'doc/Security of user switching support.html',
		'doc/Architectural overview.html'
	]
	
	USER_EXECUTABLES = [
		'passenger',
		'passenger-install-apache2-module',
		'passenger-install-nginx-module',
		'passenger-config',
		'passenger-stress-test',
		'passenger-find-frozen-apps',
		'passenger-show-backtrace',
		'passenger-archive-server'
	]
	
	SUPER_USER_EXECUTABLES = [
		'passenger-status',
		'passenger-memory-stats',
		'passenger-make-enterprisey',
		'passenger-irb'
	]
	
	# A list of globs which match all files that should be packaged
	# in the Phusion Passenger gem or tarball.
	GLOB = [
		'Rakefile',
		'README',
		'DEVELOPERS.TXT',
		'PACKAGING.TXT',
		'LICENSE',
		'INSTALL',
		'NEWS',
		'build/*.rb',
		'lib/*.rb',
		'lib/**/*.rb',
		'lib/**/*.py',
		'lib/phusion_passenger/templates/*',
		'lib/phusion_passenger/templates/apache2/*',
		'lib/phusion_passenger/templates/nginx/*',
		'lib/phusion_passenger/templates/standalone/*',
		'lib/phusion_passenger/templates/standalone_default_root/*',
		'bin/*',
		'doc/**/*',
		'man/*',
		'debian/*',
		'helper-scripts/*',
		'ext/common/**/*.{cpp,c,h,hpp}',
		'ext/apache2/*.{cpp,h,hpp,c,TXT}',
		'ext/nginx/*.{c,cpp,h}',
		'ext/nginx/config',
		'ext/boost/**/*',
		'ext/google/**/*',
		'ext/libev/{LICENSE,Changes,README,Makefile.am,Makefile.in}',
		'ext/libev/{*.m4,autogen.sh,config.guess,config.h.in,config.sub}',
		'ext/libev/{configure,configure.ac,install-sh,ltmain.sh,missing,mkinstalldirs}',
		'ext/libev/{*.h,*.c}',
		'ext/oxt/*.hpp',
		'ext/oxt/*.cpp',
		'ext/oxt/detail/*.hpp',
		'ext/ruby/*.{c,rb}',
		'misc/*',
		'test/*.example',
		'test/support/*.{cpp,h,rb}',
		'test/tut/*',
		'test/cxx/*.{cpp,h}',
		'test/oxt/*.{cpp,hpp}',
		'test/ruby/**/*',
		'test/integration_tests/**/*',
		'test/stub/**/*'
	
	# If you're running 'rake package' for the first time, then ASCII_DOCS
	# files don't exist yet, and so won't be matched by the glob.
	# So we add these filenames manually.
	] + ASCII_DOCS
	
	EXCLUDE_GLOB = [
		'test/stub/rails_apps/3.0/empty/help/**/*',
		'test/stub/*.dSYM'
	]
end

end # module PhusionPassenger