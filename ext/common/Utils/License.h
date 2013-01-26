/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2012-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

#ifndef _PASSENGER_LICENSE_H_
#define _PASSENGER_LICENSE_H_

#ifdef __cplusplus
namespace Passenger {
#endif

#define CRACK_APPEAL_MESSAGE \
	"We kindly ask you not to try to crack this software or to obtain it illegally. " \
	"We're trying to make a living out of this and the profits are used to fund the development of the " \
	"open source version of Phusion Passenger. We can only do this with your support. If you don't have " \
	"a valid license, please buy it from www.phusionpassenger.com.\n" \
	"Thank you for your support.\n" \
	"- Hongli, Ninh, and the rest of the Phusion team"

extern char *licenseKey;

char *passenger_enterprise_license_check();

#ifdef __cplusplus
} // namespace Passenger
#endif

#endif /* _PASSENGER_LICENSE_H_ */
