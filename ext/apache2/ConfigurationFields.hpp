/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

/*
 * ConfigurationFields.hpp is automatically generated from ConfigurationFields.hpp.erb,
 * using definitions from lib/phusion_passenger/apache2/config_options.rb.
 * Edits to ConfigurationFields.hpp will be lost.
 *
 * To update ConfigurationFields.hpp:
 *   rake apache2
 *
 * To force regeneration of ConfigurationFields.hpp:
 *   rm -f ext/apache2/ConfigurationFields.hpp
 *   rake ext/apache2/ConfigurationFields.hpp
 */



	/** Enable or disable Phusion Passenger. */
	Threeway enabled;
	/** Enable or disable Passenger's high performance mode. */
	Threeway highPerformance;
	/** The minimum number of application instances to keep when cleaning idle instances. */
	int minInstances;
	/** A timeout for application startup. */
	int startTimeout;
	/** The maximum number of requests that an application instance may process. */
	int maxRequests;
	/** The group that Ruby applications must run as. */
	const char *group;
	/** The user that Ruby applications must run as. */
	const char *user;
	/** The Ruby interpreter to use. */
	const char *ruby;

