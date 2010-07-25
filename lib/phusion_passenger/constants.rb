#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
	DEFAULT_FRAMEWORK_SPAWNER_MAX_IDLE_TIME = 30 * 60
	DEFAULT_APP_SPAWNER_MAX_IDLE_TIME       = 10 * 60
	
	ROOT_UID = 0
	ROOT_GID = 0
	
	PASSENGER_ANALYTICS_WEB_LOG = "PASSENGER_ANALYTICS_WEB_LOG"
	PASSENGER_TXN_ID = "PASSENGER_TXN_ID"
	PASSENGER_GROUP_NAME = "PASSENGER_GROUP_NAME"
	PASSENGER_UNION_STATION_KEY = "PASSENGER_UNION_STATION_KEY"
end
