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

/* Workaround to shut up compiler warnings about return values */
#define IGNORE_RETVAL(val) \
	do { \
		if (val == 0) { \
			printf("\n"); \
		} \
	} while (0)

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
	 * initializes global variables. See email by Jason Rust,
	 * "Not loading after installing 4.0.7", July 5 2013.
	 */
	licenseKey = NULL;
}

static FILE *
open_license_file() {
	const char *licenseData = getenv("PASSENGER_ENTERPRISE_LICENSE_DATA");
	if (licenseData != NULL && *licenseData != '\0') {
		char path[PATH_MAX] = "/tmp/passenger.XXXXXXXX";
		int fd = mkstemp(path);
		FILE *f;

		if (fd == -1) {
			int e = errno;
			fprintf(stderr, "Error: Phusion Passenger Enterprise license detected "
				"in environment variable PASSENGER_ENTERPRISE_LICENSE_DATA, "
				"but unable to create a temporary file: %s (errno=%d)\n",
				strerror(e), e);
			return NULL;
		}

		unlink(path);
		f = fdopen(fd, "r+");
		if (f != NULL) {
			size_t len = strlen(licenseData);
			size_t ret;

			ret = fwrite(licenseData, 1, len, f);
			IGNORE_RETVAL(ret);
			if (len > 0 && licenseData[len - 1] != '\n') {
				ret = fwrite("\n", 1, 1, f);
				IGNORE_RETVAL(ret);
			}
			ret = fseek(f, 0, SEEK_SET);
			IGNORE_RETVAL(ret);
		}
		return f;
	} else {
		return fopen("/etc/passenger-enterprise-license", "r");
	}
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

	f = open_license_file();
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

static int
passenger_enterprise_on_cloud_license() {
	return strstr(licenseKey, "Cloud license") != NULL;
}

static int
passenger_enterprise_on_heroku_license() {
	return strstr(licenseKey, "Heroku license") != NULL;
}

int
passenger_enterprise_should_track_usage() {
	return passenger_enterprise_on_cloud_license()
		|| passenger_enterprise_on_heroku_license();
}

#ifdef __cplusplus
} // namespace Passenger
#endif
