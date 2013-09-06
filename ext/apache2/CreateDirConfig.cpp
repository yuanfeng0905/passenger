/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

/*
 * CreateDirConfig.cpp is automatically generated from CreateDirConfig.cpp.erb,
 * using definitions from lib/phusion_passenger/apache2/config_options.rb.
 * Edits to CreateDirConfig.cpp will be lost.
 *
 * To update CreateDirConfig.cpp:
 *   rake apache2
 *
 * To force regeneration of CreateDirConfig.cpp:
 *   rm -f ext/apache2/CreateDirConfig.cpp
 *   rake ext/apache2/CreateDirConfig.cpp
 */



			config->ruby = NULL;
				config->minInstances = UNSET_INT_VALUE;
				config->user = NULL;
				config->group = NULL;
				config->maxRequests = UNSET_INT_VALUE;
				config->startTimeout = UNSET_INT_VALUE;
				config->highPerformance = DirConfig::UNSET;
				config->enabled = DirConfig::UNSET;
				config->maxRequestQueueSize = UNSET_INT_VALUE;
	