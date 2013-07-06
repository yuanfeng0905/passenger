/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2012-2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MD5.h"
#include "License.h"

#ifdef __cplusplus
namespace Passenger {
#endif

#define MAX_LICENSE_LINES 30
#define LICENSE_SECRET "An error occurred while fetching this page. Please contact an administrator if this problem persists."

char *licenseKey = (char *) 0;

static md5_byte_t
hexNibbleToByte(char hexNibble) {
	if (hexNibble >= '0' && hexNibble <= '9') {
		return hexNibble - '0';
	} else {
		return hexNibble - 'A' + 10;
	}
}

void
passenger_enterprise_license_init() {
	/* Because of bugs on certain operating systems, we cannot assume that
	 * `licenseKey` is NULL before initialization. This init function
	 * initializes global variables.
	 */
	licenseKey = NULL;
}

char *
passenger_enterprise_license_check() {
	FILE *f;
	char *lines[MAX_LICENSE_LINES];
	char line[1024];
	unsigned int count = 0, i;
	char *message = NULL;
	size_t len, totalSize = 0;
	struct md5_state_s md5;
	md5_byte_t digest[MD5_SIZE], readDigest[MD5_SIZE], *readDigestCursor;
	const char *data, *dataEnd;
	char *dataEnd2;

	if (licenseKey != NULL) {
		return strdup("Phusion Passenger Enterprise license key already checked.");
	}

	f = fopen("/etc/passenger-enterprise-license", "r");
	if (f == NULL) {
		return strdup("Could not open the Phusion Passenger Enterprise license file. "
			"Please check whether it's installed correctly and whether it's world-readable.\n"
			APPEAL_MESSAGE);
	}

	while (1) {
		if (fgets(line, sizeof(line), f) == NULL) {
			if (ferror(f)) {
				message = strdup(
					"An I/O error occurred while reading the Phusion Passenger Enterprise license file.\n"
					APPEAL_MESSAGE);
				goto finish;
			} else {
				break;
			}
		}

		len = strlen(line);
		if (len == 0 || line[len - 1] != '\n' || count >= MAX_LICENSE_LINES) {
			message = strdup("The Phusion Passenger Enterprise license file appears to be corrupted. Please reinstall it.\n"
				APPEAL_MESSAGE);
			goto finish;
		}

		lines[count] = strdup(line);
		count++;
		totalSize += len;
	}

	if (count == 0) {
		message = strdup("The Phusion Passenger Enterprise license file appears to be corrupted. Please reinstall it.\n"
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
		message = strdup("The Phusion Passenger Enterprise license file is invalid.\n"
			APPEAL_MESSAGE);
		goto finish;
	}

	licenseKey = (char *) malloc(totalSize + 1);
	dataEnd2 = licenseKey;
	for (i = 0; i < count - 1; i++) {
		len = strlen(lines[i]);
		memcpy(dataEnd2, lines[i], len);
		dataEnd2 += len;
	}
	*dataEnd2 = '\0';

	finish:
	fclose(f);
	for (i = 0; i < count; i++) {
		free(lines[i]);
	}
	return message;
}

int
passenger_enterprise_on_cloud_license() {
	return strstr(licenseKey, "Cloud license") != NULL;
}

#ifdef __cplusplus
} // namespace Passenger
#endif
