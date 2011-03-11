/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_CACHED_FILE_STAT_H_
#define _PASSENGER_CACHED_FILE_STAT_H_

#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif


/** C bindings for Passenger::CachedFileStat. */

typedef void PassengerCachedFileStat;

PassengerCachedFileStat *cached_file_stat_new(unsigned int max_size);
void cached_file_stat_free(PassengerCachedFileStat *cstat);
int  cached_file_stat_perform(PassengerCachedFileStat *cstat,
                              const char *filename,
                              struct stat *buf,
                              unsigned int throttle_rate);


#ifdef __cplusplus
}
#endif

#endif /* _PASSENGER_CACHED_FILE_STAT_H_ */
