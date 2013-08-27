/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

/*
 * ConfigurationFields.h is automatically generated from ConfigurationFields.h.erb,
 * using definitions from lib/phusion_passenger/nginx/config_options.rb.
 * Edits to MergeLocationConfig.c will be lost.
 *
 * To update ConfigurationFields.h:
 *   rake nginx
 *
 * To force regeneration of ConfigurationFields.h:
 *   rm -f ext/nginx/ConfigurationFields.h
 *   rake ext/nginx/ConfigurationFields.h
 */




	ngx_int_t enabled;

	ngx_int_t resist_deployment_errors;

	ngx_int_t rolling_restarts;

	ngx_int_t thread_count;

	ngx_int_t friendly_error_pages;

	ngx_int_t min_instances;

	ngx_int_t max_requests;

	ngx_int_t start_timeout;

	ngx_array_t *base_uris;

	ngx_int_t memory_limit;

	ngx_int_t max_request_time;

	ngx_int_t max_instances;

	ngx_array_t *vars_source;

	ngx_int_t max_preloader_idle_time;

	ngx_int_t union_station_support;

	ngx_int_t show_version_in_header;

	ngx_int_t debugger;

	ngx_array_t *union_station_filters;

	ngx_str_t app_rights;

	ngx_str_t app_root;

	ngx_str_t spawn_method;

	ngx_str_t union_station_key;

	ngx_str_t fly_with;

	ngx_str_t app_group_name;

	ngx_str_t group;

	ngx_str_t user;

	ngx_str_t concurrency_model;

	ngx_str_t environment;

	ngx_str_t python;

	ngx_str_t ruby;

