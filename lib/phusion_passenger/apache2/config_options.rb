#  Phusion Passenger - https://www.phusionpassenger.com/
#  Copyright (c) 2013 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

# This file defines all supported Apache per-directory configuration options. The
# build system automatically generates the corresponding Apache module boilerplate
# code from the definitions in this file.
# 
# Main configuration options are not defined in this file, but are defined in
# ext/apache2/Configuraion.cpp instead.
# 
# The following boilerplate code is generated:
# 
#  * command_rec array members (ConfigurationCommands.cpp.erb)
# 
# Options:
# 
#  * name - The configuration option name. Required.
#  * context - The context in which this configuration option is valid.
#              Defaults to ["OR_OPTIONS", "ACCESS_CONF", "RSRC_CONF"]
#  * type - This configuration option's value type. Allowed types:
#           :string, :integer, :flag
#  * min_value - If `type` is :integer, then this specifies the minimum
#                allowed value. When nil (the default), there is no minimum.
#  * desc - A description for this configuration option. Required.
#  * header - The name of the corresponding CGI header. By default CGI header
#             generation code is automatically generated, using the configuration
#             option's name in uppercase as the CGI header name.
#             Setting this to nil will disable auto-generation of CGI header
#             generation code. You are then responsible for writing CGI header
#             passing code yourself in Hooks.cpp.

APACHE2_DIRECTORY_CONFIGURATION_OPTIONS = [
	{
		:name => "PassengerRuby",
		:type => :string,
		:desc => "The Ruby interpreter to use.",
		:header_expression => "config->ruby ? config->ruby : serverConfig.defaultRuby"
	},
	{
		:name => "PassengerPython",
		:type => :string,
		:desc => "The Python interpreter to use."
	},
	{
		:name => "PassengerNodejs",
		:type => :string,
		:desc => "The Node.js command to use."
	},
	{
		:name => "PassengerAppEnv",
		:type => :string,
		:desc => "The environment under which applications are run."
	},
	{
		:name => "RailsEnv",
		:type => :string,
		:desc => "The environment under which applications are run.",
		:alias_for => "PassengerAppEnv"
	},
	{
		:name => "RackEnv",
		:type => :string,
		:desc => "The environment under which applications are run.",
		:alias_for => "PassengerAppEnv"
	},
	{
		:name => "PassengerMinInstances",
		:type => :integer,
		:context => ["OR_LIMIT", "ACCESS_CONF", "RSRC_CONF"],
		:min_value => 0,
		:desc => "The minimum number of application instances to keep when cleaning idle instances."
	},
	{
		:name => "PassengerUser",
		:type => :string,
		:context => ["ACCESS_CONF", "RSRC_CONF"],
		:desc => "The user that Ruby applications must run as."
	},
	{
		:name => "PassengerGroup",
		:type => :string,
		:context => ["ACCESS_CONF", "RSRC_CONF"],
		:desc => "The group that Ruby applications must run as."
	},
	{
		:name => "PassengerErrorOverride",
		:type => :flag,
		:context => ["OR_ALL"],
		:desc    => "Allow Apache to handle error response.",
		:header  => nil
	},
	{
		:name => "PassengerMaxRequests",
		:type => :integer,
		:context => ["OR_LIMIT", "ACCESS_CONF", "RSRC_CONF"],
		:min_value => 0,
		:desc => "The maximum number of requests that an application instance may process."
	},
	{
		:name => "PassengerStartTimeout",
		:type => :integer,
		:context => ["OR_LIMIT", "ACCESS_CONF", "RSRC_CONF"],
		:min_value => 1,
		:desc => "A timeout for application startup."
	},
	{
		:name => "PassengerHighPerformance",
		:type => :flag,
		:context => ["OR_ALL"],
		:desc    => "Enable or disable Passenger's high performance mode.",
		:header  => nil
	},
	{
		:name => "PassengerEnabled",
		:type => :flag,
		:context => ["OR_ALL"],
		:desc    => "Enable or disable Phusion Passenger.",
		:header  => nil
	},
	{
		:name => "PassengerMaxRequestQueueSize",
		:type => :integer,
		:min_value => 0,
		:context => ["OR_ALL"],
		:desc => "The maximum number of queued requests."
	},
	{
		:name => "PassengerLoadShellEnvvars",
		:type => :flag,
		:desc => "Whether to load environment variables from the shell before running the application."
	},
	{
		:name    => "PassengerBufferUpload",
		:type    => :flag,
		:context => ["OR_ALL"],
		:desc    => "Whether to buffer file uploads.",
		:header  => nil
	},
	{
		:name    => 'PassengerAppType',
		:type    => :string,
		:context => ["OR_ALL"],
		:desc    => "Force specific application type.",
		:header  => nil
	},
	{
		:name    => 'PassengerStartupFile',
		:context => ["OR_ALL"],
		:desc    => "Force specific startup file.",
		:type    => :string
	}
]
