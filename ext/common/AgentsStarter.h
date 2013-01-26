/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

/* C wrappers for Passenger::AgentsStarter. */
#ifndef _PASSENGER_AGENTS_STARTER_H_
#define _PASSENGER_AGENTS_STARTER_H_

#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
	extern "C" {
#endif


typedef void AgentsStarter;

typedef enum {
	AS_APACHE,
	AS_NGINX
} AgentsStarterType;

typedef void (*AfterForkCallback)(void *);

AgentsStarter *agents_starter_new(AgentsStarterType type, char **error_message);
int  agents_starter_start(AgentsStarter *as,
                          int logLevel, const char *debugLogFile,
                          pid_t webServerPid,
                          const char *tempDir, int userSwitching,
                          const char *defaultUser, const char *defaultGroup,
                          uid_t webServerWorkerUid, gid_t webServerWorkerGid,
                          const char *passengerRoot,
                          const char *rubyCommand, unsigned int maxPoolSize,
                          unsigned int maxInstancesPerApp,
                          unsigned int poolIdleTime,
                          const char *analyticsServer,
                          const char *analyticsLogUser,
                          const char *analyticsLogGroup,
                          const char *unionStationGatewayAddress,
                          unsigned short unionStationGatewayPort,
                          const char *unionStationGatewayCert,
                          const char *unionStationProxyAddress,
                          const char **prestartURLs, unsigned int prestartURLsCount,
                          const AfterForkCallback afterFork,
                          void *callbackArgument,
                          char **errorMessage);
const char *agents_starter_get_request_socket_filename(AgentsStarter *as, unsigned int *size);
const char *agents_starter_get_request_socket_password(AgentsStarter *as, unsigned int *size);
const char *agents_starter_get_server_instance_dir(AgentsStarter *as);
const char *agents_starter_get_generation_dir(AgentsStarter *as);
pid_t       agents_starter_get_pid(AgentsStarter *as);
void        agents_starter_detach(AgentsStarter *as);
void        agents_starter_free(AgentsStarter *as);


#ifdef __cplusplus
	}
#endif

#endif /* _PASSENGER_AGENTS_STARTER_H_ */
