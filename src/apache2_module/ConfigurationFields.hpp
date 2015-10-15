/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2015 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */

/*
 * ConfigurationFields.hpp is automatically generated from ConfigurationFields.hpp.erb,
 * using definitions from src/ruby_supportlib/phusion_passenger/apache2/config_options.rb.
 * Edits to ConfigurationFields.hpp will be lost.
 *
 * To update ConfigurationFields.hpp:
 *   rake apache2
 *
 * To force regeneration of ConfigurationFields.hpp:
 *   rm -f src/apache2_module/ConfigurationFields.hpp
 *   rake src/apache2_module/ConfigurationFields.hpp
 */

struct GeneratedDirConfigPart {
	enum Threeway { ENABLED, DISABLED, UNSET };


	/** Whether to buffer file uploads. */
	Threeway bufferUpload;
	/** Enable or disable Phusion Passenger. */
	Threeway enabled;
	/** Allow Apache to handle error response. */
	Threeway errorOverride;
	/** Whether to display friendly error pages when something goes wrong. */
	Threeway friendlyErrorPages;
	/** Enable or disable Passenger's high performance mode. */
	Threeway highPerformance;
	/** Whether to load environment variables from the shell before running the application. */
	Threeway loadShellEnvvars;
	/** Whether to show the Phusion Passenger version number in the X-Powered-By header. */
	Threeway showVersionInHeader;
	/** Whether to enable sticky sessions. */
	Threeway stickySessions;
	/** The cookie name to use for sticky sessions. */
	Threeway stickySessionsCookieName;
	/** The maximum number of simultaneously alive application instances a single application may occupy. */
	int maxInstancesPerApp;
	/** The maximum number of seconds that a preloader process may be idle before it is shutdown. */
	int maxPreloaderIdleTime;
	/** The maximum number of queued requests. */
	int maxRequestQueueSize;
	/** The maximum number of requests that an application instance may process. */
	int maxRequests;
	/** The minimum number of application instances to keep when cleaning idle instances. */
	int minInstances;
	/** A timeout for application startup. */
	int startTimeout;
	/** The environment under which applications are run. */
	const char *appEnv;
	/** Application process group name. */
	const char *appGroupName;
	/** Force specific application type. */
	const char *appType;
	/** The group that Ruby applications must run as. */
	const char *group;
	/** Settings file for (non-bundled) Meteor apps. */
	const char *meteorAppSettings;
	/** The Node.js command to use. */
	const char *nodejs;
	/** The Python interpreter to use. */
	const char *python;
	/** The directory in which Passenger should look for restart.txt. */
	const char *restartDir;
	/** The Ruby interpreter to use. */
	const char *ruby;
	/** The spawn method to use. */
	const char *spawnMethod;
	/** Force specific startup file. */
	const char *startupFile;
	/** The user that Ruby applications must run as. */
	const char *user;

};
