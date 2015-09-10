#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2014-2015 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'ruby_core_enhancements'

module PhusionPassenger
  module Standalone

    module ConfigUtils
      extend self    # Make methods available as class methods.

      def self.included(klass)
        # When included into another class, make sure that Utils
        # methods are made private.
        public_instance_methods(false).each do |method_name|
          klass.send(:private, method_name)
        end
      end

      class ConfigLoadError < StandardError
      end

      def global_config_file_path
        @global_config_file_path ||= File.join(PhusionPassenger.home_dir,
          USER_NAMESPACE_DIRNAME, "standalone", "config.json")
      end

      def load_local_config_file!(app_dir, options)
        config_file = File.join(app_dir, "Passengerfile.json")
        if !File.exist?(config_file)
          config_file = File.join(app_dir, "passenger-standalone.json")
        end
        if File.exist?(config_file)
          local_options = load_config_file(config_file)
          options.merge!(local_options)
        end
      end

      def load_config_file(filename)
        if !defined?(PhusionPassenger::Utils::JSON)
          PhusionPassenger.require_passenger_lib 'utils/json'
        end
        begin
          data = File.open(filename, "r:utf-8") do |f|
            f.read
          end
        rescue SystemCallError => e
          raise ConfigLoadError, "cannot load config file #{filename} (#{e})"
        end

        begin
          config = PhusionPassenger::Utils::JSON.parse(data)
        rescue => e
          raise ConfigLoadError, "cannot parse config file #{filename} (#{e})"
        end
        if !config.is_a?(Hash)
          raise ConfigLoadError, "cannot parse config file #{filename} (it does not contain an object)"
        end

        result = {}
        config.each_pair do |key, val|
          result[key.to_sym] = val
        end

        resolve_config_file_paths(result, filename)

        result
      end

      # Absolutize relative paths. Make them relative to the config file in which
      # it's specified.
      def resolve_config_file_paths(config_file_options, config_filename)
        options = config_file_options
        config_file_dir = File.dirname(File.absolute_path_no_resolve(config_filename))

        keys = [:socket_file, :ssl_certificate, :ssl_certificate_key, :log_file,
          :pid_file, :instance_registry_dir, :data_buffer_dir, :meteor_app_settings,
          :rackup, :startup_file, :static_files_dir, :restart_dir,
          :nginx_config_template]
        keys.each do |key|
          if filename = options[key]
            options[key] = File.expand_path(filename, config_file_dir)
          end
        end
      end
    end

  end # module Standalone
end # module PhusionPassenger
