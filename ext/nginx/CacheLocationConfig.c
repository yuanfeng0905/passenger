/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2014 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

/*
 * CacheLocationConfig.c is automatically generated from CacheLocationConfig.c.erb,
 * using definitions from lib/phusion_passenger/nginx/config_options.rb.
 * Edits to CacheLocationConfig.c will be lost.
 *
 * To update CacheLocationConfig.c:
 *   rake nginx
 *
 * To force regeneration of CacheLocationConfig.c:
 *   rm -f ext/nginx/CacheLocationConfig.c
 *   rake ext/nginx/CacheLocationConfig.c
 */



size_t len = 0;
u_char int_buf[32], *end, *buf, *pos;

/* Calculate lengths */

	
		if (conf->ruby.data != NULL) {
			len += sizeof("!~PASSENGER_RUBY: ") - 1;
			len += conf->ruby.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->python.data != NULL) {
			len += sizeof("!~PASSENGER_PYTHON: ") - 1;
			len += conf->python.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->nodejs.data != NULL) {
			len += sizeof("!~PASSENGER_NODEJS: ") - 1;
			len += conf->nodejs.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->environment.data != NULL) {
			len += sizeof("!~PASSENGER_APP_ENV: ") - 1;
			len += conf->environment.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->friendly_error_pages != NGX_CONF_UNSET) {
			len += sizeof("!~PASSENGER_FRIENDLY_ERROR_PAGES: ") - 1;
			len += conf->friendly_error_pages
				? sizeof("t\r\n") - 1
				: sizeof("f\r\n") - 1;
		}
	

	
		if (conf->min_instances != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->min_instances);
			len += sizeof("!~PASSENGER_MIN_PROCESSES: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->max_instances_per_app != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_instances_per_app);
			len += sizeof("!~PASSENGER_MAX_PROCESSES: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->max_requests != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_requests);
			len += sizeof("!~PASSENGER_MAX_REQUESTS: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->start_timeout != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->start_timeout);
			len += sizeof("!~PASSENGER_START_TIMEOUT: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->user.data != NULL) {
			len += sizeof("!~PASSENGER_USER: ") - 1;
			len += conf->user.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->group.data != NULL) {
			len += sizeof("!~PASSENGER_GROUP: ") - 1;
			len += conf->group.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->app_group_name.data != NULL) {
			len += sizeof("!~PASSENGER_APP_GROUP_NAME: ") - 1;
			len += conf->app_group_name.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->app_root.data != NULL) {
			len += sizeof("!~PASSENGER_APP_ROOT: ") - 1;
			len += conf->app_root.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->app_rights.data != NULL) {
			len += sizeof("!~PASSENGER_APP_RIGHTS: ") - 1;
			len += conf->app_rights.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->union_station_support != NGX_CONF_UNSET) {
			len += sizeof("!~UNION_STATION_SUPPORT: ") - 1;
			len += conf->union_station_support
				? sizeof("t\r\n") - 1
				: sizeof("f\r\n") - 1;
		}
	

	
		if (conf->debugger != NGX_CONF_UNSET) {
			len += sizeof("!~PASSENGER_DEBUGGER: ") - 1;
			len += conf->debugger
				? sizeof("t\r\n") - 1
				: sizeof("f\r\n") - 1;
		}
	

	
		if (conf->max_preloader_idle_time != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_preloader_idle_time);
			len += sizeof("!~PASSENGER_MAX_PRELOADER_IDLE_TIME: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->spawn_method.data != NULL) {
			len += sizeof("!~PASSENGER_SPAWN_METHOD: ") - 1;
			len += conf->spawn_method.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->load_shell_envvars != NGX_CONF_UNSET) {
			len += sizeof("!~PASSENGER_LOAD_SHELL_ENVVARS: ") - 1;
			len += conf->load_shell_envvars
				? sizeof("t\r\n") - 1
				: sizeof("f\r\n") - 1;
		}
	

	
		if (conf->union_station_key.data != NULL) {
			len += sizeof("!~UNION_STATION_KEY: ") - 1;
			len += conf->union_station_key.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->max_request_queue_size != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_request_queue_size);
			len += sizeof("!~PASSENGER_MAX_REQUEST_QUEUE_SIZE: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->request_queue_overflow_status_code != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->request_queue_overflow_status_code);
			len += sizeof("!~PASSENGER_REQUEST_QUEUE_OVERFLOW_STATUS_CODE: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->restart_dir.data != NULL) {
			len += sizeof("!~PASSENGER_RESTART_DIR: ") - 1;
			len += conf->restart_dir.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->startup_file.data != NULL) {
			len += sizeof("!~PASSENGER_STARTUP_FILE: ") - 1;
			len += conf->startup_file.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->sticky_sessions != NGX_CONF_UNSET) {
			len += sizeof("!~PASSENGER_STICKY_SESSIONS: ") - 1;
			len += conf->sticky_sessions
				? sizeof("t\r\n") - 1
				: sizeof("f\r\n") - 1;
		}
	

	
		if (conf->sticky_sessions_cookie_name.data != NULL) {
			len += sizeof("!~PASSENGER_STICKY_SESSIONS_COOKIE_NAME: ") - 1;
			len += conf->sticky_sessions_cookie_name.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->vary_turbocache_by_cookie.data != NULL) {
			len += sizeof("!~PASSENGER_VARY_TURBOCACHE_BY_COOKIE: ") - 1;
			len += conf->vary_turbocache_by_cookie.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->fly_with.data != NULL) {
			len += sizeof("!~PASSENGER_FLY_WITH: ") - 1;
			len += conf->fly_with.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->max_instances != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_instances);
			len += sizeof("!~PASSENGER_MAX_PROCESSES: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->max_request_time != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_request_time);
			len += sizeof("!~PASSENGER_MAX_REQUEST_TIME: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->memory_limit != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->memory_limit);
			len += sizeof("!~PASSENGER_MEMORY_LIMIT: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->concurrency_model.data != NULL) {
			len += sizeof("!~PASSENGER_CONCURRENCY_MODEL: ") - 1;
			len += conf->concurrency_model.len;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->thread_count != NGX_CONF_UNSET) {
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->thread_count);
			len += sizeof("!~PASSENGER_THREAD_COUNT: ") - 1;
			len += end - int_buf;
			len += sizeof("\r\n") - 1;
		}
	

	
		if (conf->rolling_restarts != NGX_CONF_UNSET) {
			len += sizeof("!~PASSENGER_ROLLING_RESTARTS: ") - 1;
			len += conf->rolling_restarts
				? sizeof("t\r\n") - 1
				: sizeof("f\r\n") - 1;
		}
	

	
		if (conf->resist_deployment_errors != NGX_CONF_UNSET) {
			len += sizeof("!~PASSENGER_RESIST_DEPLOYMENT_ERRORS: ") - 1;
			len += conf->resist_deployment_errors
				? sizeof("t\r\n") - 1
				: sizeof("f\r\n") - 1;
		}
	


/* Create string */
buf = pos = ngx_pnalloc(cf->pool, len);
if (buf == NULL) {
	return NGX_ERROR;
}


	
		if (conf->ruby.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_RUBY: ",
				sizeof("!~PASSENGER_RUBY: ") - 1);
			pos = ngx_copy(pos,
				conf->ruby.data,
				conf->ruby.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->python.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_PYTHON: ",
				sizeof("!~PASSENGER_PYTHON: ") - 1);
			pos = ngx_copy(pos,
				conf->python.data,
				conf->python.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->nodejs.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_NODEJS: ",
				sizeof("!~PASSENGER_NODEJS: ") - 1);
			pos = ngx_copy(pos,
				conf->nodejs.data,
				conf->nodejs.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->environment.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_APP_ENV: ",
				sizeof("!~PASSENGER_APP_ENV: ") - 1);
			pos = ngx_copy(pos,
				conf->environment.data,
				conf->environment.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->friendly_error_pages != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_FRIENDLY_ERROR_PAGES: ",
				sizeof("!~PASSENGER_FRIENDLY_ERROR_PAGES: ") - 1);
			if (conf->friendly_error_pages) {
				pos = ngx_copy(pos, "t\r\n", sizeof("t\r\n") - 1);
			} else {
				pos = ngx_copy(pos, "f\r\n", sizeof("f\r\n") - 1);
			}
		}
	

	
		if (conf->min_instances != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_MIN_PROCESSES: ",
				sizeof("!~PASSENGER_MIN_PROCESSES: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->min_instances);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->max_instances_per_app != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_MAX_PROCESSES: ",
				sizeof("!~PASSENGER_MAX_PROCESSES: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_instances_per_app);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->max_requests != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_MAX_REQUESTS: ",
				sizeof("!~PASSENGER_MAX_REQUESTS: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_requests);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->start_timeout != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_START_TIMEOUT: ",
				sizeof("!~PASSENGER_START_TIMEOUT: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->start_timeout);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->user.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_USER: ",
				sizeof("!~PASSENGER_USER: ") - 1);
			pos = ngx_copy(pos,
				conf->user.data,
				conf->user.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->group.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_GROUP: ",
				sizeof("!~PASSENGER_GROUP: ") - 1);
			pos = ngx_copy(pos,
				conf->group.data,
				conf->group.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->app_group_name.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_APP_GROUP_NAME: ",
				sizeof("!~PASSENGER_APP_GROUP_NAME: ") - 1);
			pos = ngx_copy(pos,
				conf->app_group_name.data,
				conf->app_group_name.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->app_root.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_APP_ROOT: ",
				sizeof("!~PASSENGER_APP_ROOT: ") - 1);
			pos = ngx_copy(pos,
				conf->app_root.data,
				conf->app_root.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->app_rights.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_APP_RIGHTS: ",
				sizeof("!~PASSENGER_APP_RIGHTS: ") - 1);
			pos = ngx_copy(pos,
				conf->app_rights.data,
				conf->app_rights.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->union_station_support != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~UNION_STATION_SUPPORT: ",
				sizeof("!~UNION_STATION_SUPPORT: ") - 1);
			if (conf->union_station_support) {
				pos = ngx_copy(pos, "t\r\n", sizeof("t\r\n") - 1);
			} else {
				pos = ngx_copy(pos, "f\r\n", sizeof("f\r\n") - 1);
			}
		}
	

	
		if (conf->debugger != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_DEBUGGER: ",
				sizeof("!~PASSENGER_DEBUGGER: ") - 1);
			if (conf->debugger) {
				pos = ngx_copy(pos, "t\r\n", sizeof("t\r\n") - 1);
			} else {
				pos = ngx_copy(pos, "f\r\n", sizeof("f\r\n") - 1);
			}
		}
	

	
		if (conf->max_preloader_idle_time != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_MAX_PRELOADER_IDLE_TIME: ",
				sizeof("!~PASSENGER_MAX_PRELOADER_IDLE_TIME: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_preloader_idle_time);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->spawn_method.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_SPAWN_METHOD: ",
				sizeof("!~PASSENGER_SPAWN_METHOD: ") - 1);
			pos = ngx_copy(pos,
				conf->spawn_method.data,
				conf->spawn_method.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->load_shell_envvars != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_LOAD_SHELL_ENVVARS: ",
				sizeof("!~PASSENGER_LOAD_SHELL_ENVVARS: ") - 1);
			if (conf->load_shell_envvars) {
				pos = ngx_copy(pos, "t\r\n", sizeof("t\r\n") - 1);
			} else {
				pos = ngx_copy(pos, "f\r\n", sizeof("f\r\n") - 1);
			}
		}
	

	
		if (conf->union_station_key.data != NULL) {
			pos = ngx_copy(pos,
				"!~UNION_STATION_KEY: ",
				sizeof("!~UNION_STATION_KEY: ") - 1);
			pos = ngx_copy(pos,
				conf->union_station_key.data,
				conf->union_station_key.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->max_request_queue_size != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_MAX_REQUEST_QUEUE_SIZE: ",
				sizeof("!~PASSENGER_MAX_REQUEST_QUEUE_SIZE: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_request_queue_size);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->request_queue_overflow_status_code != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_REQUEST_QUEUE_OVERFLOW_STATUS_CODE: ",
				sizeof("!~PASSENGER_REQUEST_QUEUE_OVERFLOW_STATUS_CODE: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->request_queue_overflow_status_code);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->restart_dir.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_RESTART_DIR: ",
				sizeof("!~PASSENGER_RESTART_DIR: ") - 1);
			pos = ngx_copy(pos,
				conf->restart_dir.data,
				conf->restart_dir.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->startup_file.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_STARTUP_FILE: ",
				sizeof("!~PASSENGER_STARTUP_FILE: ") - 1);
			pos = ngx_copy(pos,
				conf->startup_file.data,
				conf->startup_file.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->sticky_sessions != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_STICKY_SESSIONS: ",
				sizeof("!~PASSENGER_STICKY_SESSIONS: ") - 1);
			if (conf->sticky_sessions) {
				pos = ngx_copy(pos, "t\r\n", sizeof("t\r\n") - 1);
			} else {
				pos = ngx_copy(pos, "f\r\n", sizeof("f\r\n") - 1);
			}
		}
	

	
		if (conf->sticky_sessions_cookie_name.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_STICKY_SESSIONS_COOKIE_NAME: ",
				sizeof("!~PASSENGER_STICKY_SESSIONS_COOKIE_NAME: ") - 1);
			pos = ngx_copy(pos,
				conf->sticky_sessions_cookie_name.data,
				conf->sticky_sessions_cookie_name.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->vary_turbocache_by_cookie.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_VARY_TURBOCACHE_BY_COOKIE: ",
				sizeof("!~PASSENGER_VARY_TURBOCACHE_BY_COOKIE: ") - 1);
			pos = ngx_copy(pos,
				conf->vary_turbocache_by_cookie.data,
				conf->vary_turbocache_by_cookie.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->fly_with.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_FLY_WITH: ",
				sizeof("!~PASSENGER_FLY_WITH: ") - 1);
			pos = ngx_copy(pos,
				conf->fly_with.data,
				conf->fly_with.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->max_instances != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_MAX_PROCESSES: ",
				sizeof("!~PASSENGER_MAX_PROCESSES: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_instances);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->max_request_time != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_MAX_REQUEST_TIME: ",
				sizeof("!~PASSENGER_MAX_REQUEST_TIME: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->max_request_time);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->memory_limit != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_MEMORY_LIMIT: ",
				sizeof("!~PASSENGER_MEMORY_LIMIT: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->memory_limit);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->concurrency_model.data != NULL) {
			pos = ngx_copy(pos,
				"!~PASSENGER_CONCURRENCY_MODEL: ",
				sizeof("!~PASSENGER_CONCURRENCY_MODEL: ") - 1);
			pos = ngx_copy(pos,
				conf->concurrency_model.data,
				conf->concurrency_model.len);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->thread_count != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_THREAD_COUNT: ",
				sizeof("!~PASSENGER_THREAD_COUNT: ") - 1);
			end = ngx_snprintf(int_buf,
				sizeof(int_buf) - 1,
				"%d",
				conf->thread_count);
			pos = ngx_copy(pos, int_buf, end - int_buf);
			pos = ngx_copy(pos, (const u_char *) "\r\n", sizeof("\r\n") - 1);
		}
	

	
		if (conf->rolling_restarts != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_ROLLING_RESTARTS: ",
				sizeof("!~PASSENGER_ROLLING_RESTARTS: ") - 1);
			if (conf->rolling_restarts) {
				pos = ngx_copy(pos, "t\r\n", sizeof("t\r\n") - 1);
			} else {
				pos = ngx_copy(pos, "f\r\n", sizeof("f\r\n") - 1);
			}
		}
	

	
		if (conf->resist_deployment_errors != NGX_CONF_UNSET) {
			pos = ngx_copy(pos,
				"!~PASSENGER_RESIST_DEPLOYMENT_ERRORS: ",
				sizeof("!~PASSENGER_RESIST_DEPLOYMENT_ERRORS: ") - 1);
			if (conf->resist_deployment_errors) {
				pos = ngx_copy(pos, "t\r\n", sizeof("t\r\n") - 1);
			} else {
				pos = ngx_copy(pos, "f\r\n", sizeof("f\r\n") - 1);
			}
		}
	


conf->options_cache.data = buf;
conf->options_cache.len = pos - buf;
