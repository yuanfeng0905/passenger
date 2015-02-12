#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2014 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

auto_generated_sources = [
  'ext/nginx/ConfigurationCommands.c',
  'ext/nginx/CreateLocationConfig.c',
  'ext/nginx/MergeLocationConfig.c',
  'ext/nginx/CacheLocationConfig.c',
  'ext/nginx/ConfigurationFields.h'
]

desc "Build Nginx support files"
task :nginx => [
  :nginx_without_native_support,
  NATIVE_SUPPORT_TARGET
].compact

# Workaround for https://github.com/jimweirich/rake/issues/274
task :_nginx => :nginx

task :nginx_without_native_support => [
  auto_generated_sources,
  AGENT_OUTPUT_DIR + AGENT_EXE,
  COMMON_LIBRARY.only(*NGINX_LIBS_SELECTOR).link_objects
].flatten

task :clean => 'nginx:clean'
desc "Clean all compiled Nginx files"
task 'nginx:clean' => 'common:clean' do
  # Nothing to clean at this time.
end

def create_nginx_auto_generated_source_task(source)
  dependencies = [
    "#{source}.erb",
    'lib/phusion_passenger/nginx/config_options.rb'
  ]
  file(source => dependencies) do
    template = TemplateRenderer.new("#{source}.erb")
    template.render_to(source)
  end
end

auto_generated_sources.each do |source|
  create_nginx_auto_generated_source_task(source)
end
