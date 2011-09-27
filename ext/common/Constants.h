/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010, 2011 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_CONSTANTS_H_
#define _PASSENGER_CONSTANTS_H_

/* Don't forget to update lib/phusion_passenger.rb too. */
#define PASSENGER_VERSION "3.0.9"

#define FEEDBACK_FD 3

#define DEFAULT_LOG_LEVEL 0
#define DEFAULT_MAX_POOL_SIZE 6
#define DEFAULT_POOL_IDLE_TIME 300
#define DEFAULT_MAX_INSTANCES_PER_APP 0
#define DEFAULT_WEB_APP_USER "nobody"
#define DEFAULT_ANALYTICS_LOG_USER DEFAULT_WEB_APP_USER
#define DEFAULT_ANALYTICS_LOG_GROUP ""
#define DEFAULT_ANALYTICS_LOG_PERMISSIONS "u=rwx,g=rx,o=rx"
#define DEFAULT_UNION_STATION_GATEWAY_ADDRESS "gateway.unionstationapp.com"
#define DEFAULT_UNION_STATION_GATEWAY_PORT 443

#define MESSAGE_SERVER_MAX_USERNAME_SIZE 100
#define MESSAGE_SERVER_MAX_PASSWORD_SIZE 100
#define DEFAULT_BACKEND_ACCOUNT_RIGHTS Account::DETACH

#endif /* _PASSENGER_CONSTANTS_H */
