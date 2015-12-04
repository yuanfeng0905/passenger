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
 * CreateDirConfig.cpp is automatically generated from CreateDirConfig.cpp.erb,
 * using definitions from src/ruby_supportlib/phusion_passenger/apache2/config_options.rb.
 * Edits to CreateDirConfig.cpp will be lost.
 *
 * To update CreateDirConfig.cpp:
 *   rake apache2
 *
 * To force regeneration of CreateDirConfig.cpp:
 *   rm -f src/apache2_module/CreateDirConfig.cpp
 *   rake src/apache2_module/CreateDirConfig.cpp
 */



			config->ruby = NULL;
				config->python = NULL;
				config->nodejs = NULL;
				config->meteorAppSettings = NULL;
				config->appEnv = NULL;
				config->minInstances = UNSET_INT_VALUE;
				config->maxInstancesPerApp = UNSET_INT_VALUE;
				config->user = NULL;
				config->group = NULL;
				config->errorOverride = DirConfig::UNSET;
				config->maxRequests = UNSET_INT_VALUE;
				config->startTimeout = UNSET_INT_VALUE;
				config->highPerformance = DirConfig::UNSET;
				config->enabled = DirConfig::UNSET;
				config->maxRequestQueueSize = UNSET_INT_VALUE;
				config->maxPreloaderIdleTime = UNSET_INT_VALUE;
				config->loadShellEnvvars = DirConfig::UNSET;
				config->bufferUpload = DirConfig::UNSET;
				config->appType = NULL;
				config->startupFile = NULL;
				config->stickySessions = DirConfig::UNSET;
				config->stickySessionsCookieName = DirConfig::UNSET;
				config->spawnMethod = NULL;
				config->showVersionInHeader = DirConfig::UNSET;
				config->friendlyErrorPages = DirConfig::UNSET;
				config->restartDir = NULL;
				config->appGroupName = NULL;
				config->forceMaxConcurrentRequestsPerProcess = UNSET_INT_VALUE;
	
