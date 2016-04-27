/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2013 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */

/*
 * MergeDirConfig.cpp is automatically generated from MergeDirConfig.cpp.erb,
 * using definitions from src/ruby_supportlib/phusion_passenger/apache2/config_options.rb.
 * Edits to MergeDirConfig.cpp will be lost.
 *
 * To update MergeDirConfig.cpp:
 *   rake apache2
 *
 * To force regeneration of MergeDirConfig.cpp:
 *   rm -f src/apache2_module/MergeDirConfig.cpp
 *   rake src/apache2_module/MergeDirConfig.cpp
 */




	
		config->ruby =
			(add->ruby == NULL) ?
			base->ruby :
			add->ruby;
	

	
		config->python =
			(add->python == NULL) ?
			base->python :
			add->python;
	

	
		config->nodejs =
			(add->nodejs == NULL) ?
			base->nodejs :
			add->nodejs;
	

	
		config->meteorAppSettings =
			(add->meteorAppSettings == NULL) ?
			base->meteorAppSettings :
			add->meteorAppSettings;
	

	
		config->appEnv =
			(add->appEnv == NULL) ?
			base->appEnv :
			add->appEnv;
	

	
		config->minInstances =
			(add->minInstances == UNSET_INT_VALUE) ?
			base->minInstances :
			add->minInstances;
	

	
		config->maxInstancesPerApp =
			(add->maxInstancesPerApp == UNSET_INT_VALUE) ?
			base->maxInstancesPerApp :
			add->maxInstancesPerApp;
	

	
		config->user =
			(add->user == NULL) ?
			base->user :
			add->user;
	

	
		config->group =
			(add->group == NULL) ?
			base->group :
			add->group;
	

	
		config->errorOverride =
			(add->errorOverride == DirConfig::UNSET) ?
			base->errorOverride :
			add->errorOverride;
	

	
		config->maxRequests =
			(add->maxRequests == UNSET_INT_VALUE) ?
			base->maxRequests :
			add->maxRequests;
	

	
		config->startTimeout =
			(add->startTimeout == UNSET_INT_VALUE) ?
			base->startTimeout :
			add->startTimeout;
	

	
		config->highPerformance =
			(add->highPerformance == DirConfig::UNSET) ?
			base->highPerformance :
			add->highPerformance;
	

	
		config->enabled =
			(add->enabled == DirConfig::UNSET) ?
			base->enabled :
			add->enabled;
	

	
		config->maxRequestQueueSize =
			(add->maxRequestQueueSize == UNSET_INT_VALUE) ?
			base->maxRequestQueueSize :
			add->maxRequestQueueSize;
	

	
		config->maxPreloaderIdleTime =
			(add->maxPreloaderIdleTime == UNSET_INT_VALUE) ?
			base->maxPreloaderIdleTime :
			add->maxPreloaderIdleTime;
	

	
		config->loadShellEnvvars =
			(add->loadShellEnvvars == DirConfig::UNSET) ?
			base->loadShellEnvvars :
			add->loadShellEnvvars;
	

	
		config->bufferUpload =
			(add->bufferUpload == DirConfig::UNSET) ?
			base->bufferUpload :
			add->bufferUpload;
	

	
		config->appType =
			(add->appType == NULL) ?
			base->appType :
			add->appType;
	

	
		config->startupFile =
			(add->startupFile == NULL) ?
			base->startupFile :
			add->startupFile;
	

	
		config->stickySessions =
			(add->stickySessions == DirConfig::UNSET) ?
			base->stickySessions :
			add->stickySessions;
	

	
		config->stickySessionsCookieName =
			(add->stickySessionsCookieName == DirConfig::UNSET) ?
			base->stickySessionsCookieName :
			add->stickySessionsCookieName;
	

	
		config->spawnMethod =
			(add->spawnMethod == NULL) ?
			base->spawnMethod :
			add->spawnMethod;
	

	
		config->showVersionInHeader =
			(add->showVersionInHeader == DirConfig::UNSET) ?
			base->showVersionInHeader :
			add->showVersionInHeader;
	

	
		config->friendlyErrorPages =
			(add->friendlyErrorPages == DirConfig::UNSET) ?
			base->friendlyErrorPages :
			add->friendlyErrorPages;
	

	
		config->restartDir =
			(add->restartDir == NULL) ?
			base->restartDir :
			add->restartDir;
	

	
		config->appGroupName =
			(add->appGroupName == NULL) ?
			base->appGroupName :
			add->appGroupName;
	

	
		config->forceMaxConcurrentRequestsPerProcess =
			(add->forceMaxConcurrentRequestsPerProcess == UNSET_INT_VALUE) ?
			base->forceMaxConcurrentRequestsPerProcess :
			add->forceMaxConcurrentRequestsPerProcess;
	

	
		config->lveMinUid =
			(add->lveMinUid == UNSET_INT_VALUE) ?
			base->lveMinUid :
			add->lveMinUid;
	

