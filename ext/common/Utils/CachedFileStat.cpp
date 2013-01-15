/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#include "CachedFileStat.h"
#include "CachedFileStat.hpp"

extern "C" {

PassengerCachedFileStat *
cached_file_stat_new(unsigned int max_size) {
	return (PassengerCachedFileStat *) new Passenger::CachedFileStat(max_size);
}

void
cached_file_stat_free(PassengerCachedFileStat *cstat) {
	delete (Passenger::CachedFileStat *) cstat;
}

int
cached_file_stat_perform(PassengerCachedFileStat *cstat,
                         const char *filename,
                         struct stat *buf,
                         unsigned int throttle_rate) {
	try {
		return ((Passenger::CachedFileStat *) cstat)->stat(filename, buf, throttle_rate);
	} catch (const Passenger::TimeRetrievalException &e) {
		errno = e.code();
		return -1;
	} catch (const boost::thread_interrupted &) {
		errno = EINTR;
		return -1;
	}
}

} // extern "C"
