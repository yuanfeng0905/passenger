/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

/*
 * SetHeaders.cpp is automatically generated from SetHeaders.cpp.erb,
 * using definitions from lib/phusion_passenger/apache2/config_options.rb.
 * Edits to SetHeaders.cpp will be lost.
 *
 * To update SetHeaders.cpp:
 *   rake apache2
 *
 * To force regeneration of SetHeaders.cpp:
 *   rm -f ext/apache2/SetHeaders.cpp
 *   rake ext/apache2/SetHeaders.cpp
 */




	
		addHeader(output, "PASSENGER_RUBY", config->ruby ? config->ruby : serverConfig.defaultRuby);
	

	
		addHeader(output, "PASSENGER_PYTHON", config->python);
	

	
		addHeader(output, "PASSENGER_NODEJS", config->nodejs);
	

	
		addHeader(r, output, "PASSENGER_MIN_INSTANCES", config->minInstances);
	

	
		addHeader(output, "PASSENGER_USER", config->user);
	

	
		addHeader(output, "PASSENGER_GROUP", config->group);
	

	
		addHeader(r, output, "PASSENGER_MAX_REQUESTS", config->maxRequests);
	

	
		addHeader(r, output, "PASSENGER_START_TIMEOUT", config->startTimeout);
	

	
		addHeader(r, output, "PASSENGER_MAX_REQUEST_QUEUE_SIZE", config->maxRequestQueueSize);
	

	
		addHeader(r, output, "PASSENGER_LOAD_SHELL_ENVVARS", config->loadShellEnvvars);
	

	
		addHeader(output, "PASSENGER_STARTUP_FILE", config->startupFile);
	

