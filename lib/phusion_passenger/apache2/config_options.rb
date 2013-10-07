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

APACHE2_DIRECTORY_CONFIGURATION_OPTIONS = [
	{
		:name => "PassengerRuby",
		:type => :string,
		:desc => "The Ruby interpreter to use."
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
		:desc => "Enable or disable Passenger's high performance mode."
	},
	{
		:name => "PassengerEnabled",
		:type => :flag,
		:context => ["OR_ALL"],
		:desc => "Enable or disable Phusion Passenger."
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
	}
]
