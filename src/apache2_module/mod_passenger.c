/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010, 2011, 2012 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */
#include <httpd.h>
#include <http_config.h>
#include "Configuration.h"
#include "Hooks.h"

#ifdef VISIBILITY_ATTRIBUTE_SUPPORTED
	#define PUBLIC_SYMBOL __attribute__ ((visibility("default")))
#else
	#define PUBLIC_SYMBOL
#endif

PUBLIC_SYMBOL module AP_MODULE_DECLARE_DATA passenger_module = {
	STANDARD20_MODULE_STUFF,
	passenger_config_create_dir,        /* create per-dir config structs */
	passenger_config_merge_dir,         /* merge per-dir config structs */
	NULL,                               /* create per-server config structs */
	NULL,                               /* merge per-server config structs */
	passenger_commands,                 /* table of config file commands */
	passenger_register_hooks,           /* register hooks */
};
