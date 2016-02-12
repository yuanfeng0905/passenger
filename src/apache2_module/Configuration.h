/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2016 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_CONFIGURATION_H_
#define _PASSENGER_CONFIGURATION_H_

#include <apr_pools.h>
#include <httpd.h>
#include <http_config.h>

#ifdef __cplusplus
	extern "C" {
#endif


/** Configuration hook for per-directory configuration structure creation. */
void *passenger_config_create_dir(apr_pool_t *p, char *dirspec);

/** Configuration hook for per-directory configuration structure merging. */
void *passenger_config_merge_dir(apr_pool_t *p, void *basev, void *addv);

void passenger_postprocess_config(server_rec *s);

/** Apache module commands array. */
extern const command_rec passenger_commands[];


#ifdef __cplusplus
	}
#endif

#endif /* _PASSENGER_CONFIGURATION_H_ */
