#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010, 2011, 2012 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

desc "Generate all documentation"
task :doc => Packaging::ASCII_DOCS

Packaging::ASCII_DOCS.each do |target|
	source = target.sub(/\.html$/, '.txt')
	file target => [source] + Dir["doc/users_guide_snippets/**/*"] do
		if PlatformInfo.find_command('mizuho')
			if target =~ /apache/i
				type = "apache"
				juvia_site_key = "5jpmkyjqlml8rktsfldfpbwth8ig7w9"
			elsif target =~ /nginx/i
				type = "nginx"
				juvia_site_key = "q0ptarhn8o9xanwomq8zkgewbtwffyz"
			elsif target =~ /standalone/i
				type = "standalone"
				juvia_site_key = "amggdy0k65hb4hbjg3dh7pnb9zd8dwy"
			else
				type = nil
				juvia_site_key = nil
			end
			command = "mizuho '#{source}'"
			command << " -a #{type}" if type
			if juvia_site_key
				command << " -c juvia --juvia-url http://juvia.phusion.nl --juvia-site-key #{juvia_site_key}"
			end
			sh(command)
		else
			sh "echo 'Mizuho required to build docs' > '#{target}'"
		end
	end
	
	task :clean do
		if boolean_option('CLEAN_DOCS', true)
			sh "rm -f '#{target}'"
		end
	end
end

def create_markdown_compilation_task(target)
	source = target.sub(/\.html$/, '.txt.md')
	dependencies = [
		source,
		'doc/templates/markdown.html.erb',
		'doc/templates/bootstrap.min.css'
	]

	file(target => dependencies) do
		sh "bluecloth -f #{source} > #{target}.tmp"
		begin
			puts "Creating #{target}"
			require 'erb'
			template = ERB.new(File.read('doc/templates/markdown.html.erb'))
			title    = File.basename(target, '.html')
			content  = File.read("#{target}.tmp")
			css      = File.read('doc/templates/bootstrap.min.css')
			data = template.result(binding)
			File.open(target, 'w') do |f|
				f.write(data)
			end
		ensure
			sh "rm -f #{target}.tmp"
		end
	end

	task :clean do
		if boolean_option('CLEAN_DOCS', true)
			sh "rm -f #{target}"
		end
	end
end

create_markdown_compilation_task('doc/Packaging.html')
