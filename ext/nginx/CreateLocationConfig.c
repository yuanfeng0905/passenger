/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

/*
 * CreateLocationConfig.c is automatically generated from CreateLocationConfig.c.erb,
 * using definitions from lib/phusion_passenger/nginx/config_options.rb.
 * Edits to CreateLocationConfig.c will be lost.
 *
 * To update CreateLocationConfig.c:
 *   rake nginx
 *
 * To force regeneration of CreateLocationConfig.c:
 *   rm -f ext/nginx/CreateLocationConfig.c
 *   rake ext/nginx/CreateLocationConfig.c
 */




	
		conf->enabled = NGX_CONF_UNSET;
	

	
		conf->ruby.data = NULL;
		conf->ruby.len  = 0;
	

	
		conf->python.data = NULL;
		conf->python.len  = 0;
	

	
		conf->environment.data = NULL;
		conf->environment.len  = 0;
	

	
		conf->friendly_error_pages = NGX_CONF_UNSET;
	

	
		conf->min_instances = NGX_CONF_UNSET;
	

	
		conf->max_requests = NGX_CONF_UNSET;
	

	
		conf->base_uris = NGX_CONF_UNSET_PTR;
	

	
		conf->user.data = NULL;
		conf->user.len  = 0;
	

	
		conf->group.data = NULL;
		conf->group.len  = 0;
	

	
		conf->app_group_name.data = NULL;
		conf->app_group_name.len  = 0;
	

	
		conf->app_root.data = NULL;
		conf->app_root.len  = 0;
	

	
		conf->app_rights.data = NULL;
		conf->app_rights.len  = 0;
	

	
		conf->union_station_support = NGX_CONF_UNSET;
	

	
		conf->union_station_filters = NGX_CONF_UNSET_PTR;
	

	
		conf->debugger = NGX_CONF_UNSET;
	

	
		conf->show_version_in_header = NGX_CONF_UNSET;
	

	
		conf->max_preloader_idle_time = NGX_CONF_UNSET;
	

	
		conf->vars_source = NGX_CONF_UNSET_PTR;
	

	
		conf->spawn_method.data = NULL;
		conf->spawn_method.len  = 0;
	

	
		conf->union_station_key.data = NULL;
		conf->union_station_key.len  = 0;
	

	
		conf->fly_with.data = NULL;
		conf->fly_with.len  = 0;
	

	
		conf->max_instances = NGX_CONF_UNSET;
	

	
		conf->max_request_time = NGX_CONF_UNSET;
	

	
		conf->memory_limit = NGX_CONF_UNSET;
	

	
		conf->concurrency_model.data = NULL;
		conf->concurrency_model.len  = 0;
	

	
		conf->thread_count = NGX_CONF_UNSET;
	

	
		conf->rolling_restarts = NGX_CONF_UNSET;
	

	
		conf->resist_deployment_errors = NGX_CONF_UNSET;
	
