#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2015 Phusion Holding B.V.
#
#  "Passenger", "Phusion Passenger" and "Union Station" are registered
#  trademarks of Phusion Holding B.V.
#
#  See LICENSE file for license information.

auto_generated_sources = %w(
  src/nginx_module/ConfigurationCommands.c
  src/nginx_module/CreateLocationConfig.c
  src/nginx_module/MergeLocationConfig.c
  src/nginx_module/CacheLocationConfig.c
  src/nginx_module/ConfigurationFields.h
)

desc "Build Nginx support files"
task :nginx => [
  :nginx_without_native_support,
  NATIVE_SUPPORT_TARGET
].compact

# Workaround for https://github.com/jimweirich/rake/issues/274
task :_nginx => :nginx

task :nginx_without_native_support => [
  auto_generated_sources,
  AGENT_TARGET,
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
    'src/ruby_supportlib/phusion_passenger/nginx/config_options.rb'
  ]
  file(source => dependencies) do
    template = TemplateRenderer.new("#{source}.erb")
    template.render_to(source)
  end
end

auto_generated_sources.each do |source|
  create_nginx_auto_generated_source_task(source)
end
