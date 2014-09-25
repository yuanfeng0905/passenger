/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_HASHER_H_
#define _PASSENGER_HASHER_H_

#include <boost/cstdint.hpp>

namespace Passenger {


// TODO: use SpookyHash on x86_64
// TODO: use streaming murmurhash implementation: https://github.com/c9/murmur3

struct JenkinsHash {
	boost::uint32_t hash;

	JenkinsHash()
		: hash(0)
		{ }

	void update(const char *data, unsigned int size);
	boost::uint32_t finalize();

	void reset() {
		hash = 0;
	}
};

typedef JenkinsHash Hasher;


} // namespace Passenger

#endif /* _PASSENGER_HASHER_H_ */
