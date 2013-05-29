/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2012 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "MD5.h"

#ifdef __cplusplus
namespace Passenger {
#endif

#define MAX_LICENSE_LINES 30
#define LICENSE_SECRET "An error occurred while fetching this page. Please contact an administrator if this problem persists."
#define APPEAL_MESSAGE \
	"If you have a valid license, then you probably did not copy the license key properly. " \
	"The license key must be an exact byte-by-byte copy of the one downloaded from the Customer Area. " \
	"It is already considered corrupted if you add a newline to it, which can happen if you e.g. copy-paste the contents using a text editor. " \
	"Please use a binary copying tool to copy the license key to your server, e.g. 'scp' or FTP in binary mode. " \
	"If the problem persists, please contact support@phusion.nl.\n" \
	"If you do not have a valid license, then we kindly ask you not to try to crack this software or to obtain it illegally. " \
	"We're trying to make a living out of this and the profits are used to fund the development of the " \
	"open source version of Phusion Passenger. We can only do this with your support. If you don't have " \
	"a valid license, please buy it from www.phusionpassenger.com.\n" \
	"Thank you for your support.\n" \
	"- Hongli, Ninh, and the rest of the Phusion team"

static md5_byte_t
hexNibbleToByte(char hexNibble) {
	if (hexNibble >= '0' && hexNibble <= '9') {
		return hexNibble - '0';
	} else {
		return hexNibble - 'A' + 10;
	}
}

static char *
passenger_enterprise_license_check() {
	FILE *f;
	char *lines[MAX_LICENSE_LINES];
	char line[1024];
	unsigned int count = 0, i;
	char *message = NULL;
	size_t len;
	struct md5_state_s md5;
	md5_byte_t digest[MD5_SIZE], readDigest[MD5_SIZE], *readDigestCursor;
	const char *data, *dataEnd;

	f = fopen("/etc/passenger-enterprise-license", "r");
	if (f == NULL) {
		return strdup("Could not open the Passenger Enterprise Server license file. "
			"Please check whether it's installed correctly and whether it's world-readable.\n"
			APPEAL_MESSAGE);
	}

	while (1) {
		if (fgets(line, sizeof(line), f) == NULL) {
			if (ferror(f)) {
				message = strdup(
					"An I/O error occurred while reading the Passenger Enterprise Server license file.\n"
					APPEAL_MESSAGE);
				goto finish;
			} else {
				break;
			}
		}

		len = strlen(line);
		if (len == 0 || line[len - 1] != '\n' || count >= MAX_LICENSE_LINES) {
			message = strdup("The Passenger Enterprise Server license file appears to be corrupted. Please reinstall it.\n"
				APPEAL_MESSAGE);
			goto finish;
		}

		lines[count] = strdup(line);
		count++;
	}

	if (count == 0) {
		message = strdup("The Passenger Enterprise Server license file appears to be corrupted. Please reinstall it.\n"
			APPEAL_MESSAGE);
		goto finish;
	}

	md5_init(&md5);
	for (i = 0; i < count - 1; i++) {
		md5_append(&md5, (const md5_byte_t *) lines[i], strlen(lines[i]));
	}
	md5_append(&md5, (const md5_byte_t *) LICENSE_SECRET, sizeof(LICENSE_SECRET) - 1);
	md5_finish(&md5, digest);

	readDigestCursor = readDigest;
	data = lines[count - 1];
	dataEnd = data + strlen(data);
	while (data < dataEnd && *data != '\n' && readDigestCursor < readDigest + MD5_SIZE) {
		while (*data == ' ') {
			data++;
		}
		*readDigestCursor = (hexNibbleToByte(data[0]) << 4) | hexNibbleToByte(data[1]);
		readDigestCursor++;
		data += 2;
	}

	if (memcmp(digest, readDigest, MD5_SIZE) != 0) {
		message = strdup("The Passenger Enterprise Server license file is invalid.\n"
			APPEAL_MESSAGE);
		goto finish;
	}

	finish:
	fclose(f);
	for (i = 0; i < count; i++) {
		free(lines[i]);
	}
	return message;
}

#ifdef __cplusplus
} // namespace Passenger
#endif
