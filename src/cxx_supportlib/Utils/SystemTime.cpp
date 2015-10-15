/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */

#include <time.h>

namespace Passenger {
	namespace SystemTimeData {
		bool hasForcedValue = false;
		time_t forcedValue = 0;
		bool hasForcedMsecValue = false;
		unsigned long long forcedMsecValue = 0;
		bool hasForcedUsecValue = false;
		unsigned long long forcedUsecValue = 0;
	}
}
