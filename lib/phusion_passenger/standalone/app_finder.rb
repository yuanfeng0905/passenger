#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2010-2015 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

PhusionPassenger.require_passenger_lib 'ruby_core_enhancements'
PhusionPassenger.require_passenger_lib 'standalone/config_utils'
PhusionPassenger.require_passenger_lib 'utils/file_system_watcher'

module PhusionPassenger
  module Standalone

    class AppFinder
      STARTUP_FILES = [
        "config.ru",
        "passenger_wsgi.py",
        "app.js",
        ".meteor"
      ]
      WATCH_ENTRIES = [
        "config", "Passengerfile.json", "passenger-standalone.json"
      ]

      attr_accessor :dirs
      attr_reader :apps
      attr_reader :execution_root

      def self.looks_like_app_directory?(dir, options = {})
        options = options.dup
        ConfigUtils.load_local_config_file!(dir, options)
        return options[:app_type] ||
          STARTUP_FILES.any? do |file|
            File.exist?("#{dir}/#{file}")
          end
      end

      def self.supports_multi?
        true
      end

      def initialize(dirs, options = {})
        @dirs = dirs
        @options = options.dup
        determine_mode_and_execution_root(options)
      end

      def scan
        apps = []
        watchlist = []

        if single_mode?
          app_root = find_app_root
          apps << {
            :server_names => ["_"],
            :root => app_root
          }
          watchlist << app_root
          WATCH_ENTRIES.each do |entry|
            if File.exist?("#{app_root}/#{entry}")
              watchlist << "#{app_root}/#{entry}"
            end
          end

          apps.map! do |app|
            @options.merge(app)
          end
        else
          dirs = @dirs.empty? ? [File.absolute_path_no_resolve(".")] : @dirs
          dirs.each do |dir|
            if looks_like_app_directory?(dir, @options)
              app_root = File.absolute_path_no_resolve(dir)
              server_names = filename_to_server_names(dir)
              apps << {
                :server_names => server_names,
                :root => app_root
              }
              watchlist << app_root
              WATCH_ENTRIES.each do |entry|
                if File.exist?("#{app_root}/#{entry}")
                  watchlist << "#{app_root}/#{entry}"
                end
              end
            else
              full_dir = File.absolute_path_no_resolve(dir)
              watchlist << full_dir
              Dir["#{full_dir}/*"].each do |subdir|
                if looks_like_app_directory?(subdir, @options)
                  server_names = filename_to_server_names(subdir)
                  apps << {
                    :server_names => server_names,
                    :root => subdir
                  }
                end
                watchlist << subdir
                WATCH_ENTRIES.each do |entry|
                  if File.exist?("#{subdir}/#{entry}")
                    watchlist << "#{subdir}/#{entry}"
                  end
                end
              end
            end
          end

          apps.sort! do |a, b|
            a[:root] <=> b[:root]
          end
          apps.map! do |app|
            ConfigUtils.load_local_config_file!(app[:root], app)
            @options.merge(app)
          end
        end

        @apps = apps
        @watchlist = watchlist
        return apps
      end

      def monitor(termination_pipe)
        raise "You must call #scan first" if !@apps

        watcher = PhusionPassenger::Utils::FileSystemWatcher.new(@watchlist, termination_pipe)
        if wait_on_io(termination_pipe, 3)
          return
        end

        while true
          changed = watcher.wait_for_change
          watcher.close
          if changed
            old_apps = @apps
            # The change could be caused by a write to some Passengerfile.json file.
            # Wait for a short period so that the write has a chance to finish.
            if wait_on_io(termination_pipe, 0.25)
              return
            end

            begin
              new_apps = scan
            rescue AppFinder::ConfigLoadError => e
              STDERR.puts " *** ERROR: #{e}"
              new_apps = old_apps
            end
            watcher = PhusionPassenger::Utils::FileSystemWatcher.new(@watchlist, termination_pipe)
            if old_apps != new_apps
              yield(new_apps)
            end

            # Don't process change events again for a short while,
            # but do detect changes while waiting.
            if wait_on_io(termination_pipe, 3)
              return
            end
          else
            return
          end
        end
      ensure
        watcher.close if watcher
      end

      def single_mode?
        return @mode == :single
      end

      def multi_mode?
        return !single_mode?
      end

      ##################

    private
      class ConfigLoadError < StandardError
      end

      def find_app_root
        if @dirs.empty?
          return File.absolute_path_no_resolve(".")
        else
          return File.absolute_path_no_resolve(@dirs[0])
        end
      end

      def looks_like_app_directory?(dir, options = {})
        return AppFinder.looks_like_app_directory?(dir, options)
      end

      def filename_to_server_names(filename)
        basename = File.basename(filename)
        names = [basename]
        if basename !~ /^www\.$/i
          names << "www.#{basename}"
        end
        return names
      end

      # Wait until the given IO becomes readable, or until the timeout has
      # been reached. Returns true if the IO became readable, false if the
      # timeout has been reached.
      def wait_on_io(io, timeout)
        return !!select([io], nil, nil, timeout)
      end

      def determine_mode_and_execution_root(options)
        single = (@dirs.empty? && looks_like_app_directory?(".", options)) ||
          (@dirs.size == 1 && looks_like_app_directory?(@dirs[0], options))
        @mode = single ? :single : :multi
        if @dirs.empty?
          @execution_root = Dir.pwd_no_resolve
        elsif @dirs.size == 1
          @execution_root = File.absolute_path_no_resolve(@dirs[0])
        else
          @execution_root = Dir.pwd_no_resolve
        end
      end

      ##################
    end

  end # module Standalone
end # module PhusionPassenger
