/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_APPLICATION_POOL_SPAWNER_OBJECT_H_
#define _PASSENGER_APPLICATION_POOL_SPAWNER_OBJECT_H_

#include <boost/noncopyable.hpp>
#include <boost/move/core.hpp>
#include <MemoryKit/palloc.h>
#include <ApplicationPool2/Common.h>

namespace Passenger {
namespace ApplicationPool2 {


class SpawnObject: public boost::noncopyable {
private:
	BOOST_MOVABLE_BUT_NOT_COPYABLE(SpawnObject)

public:
	psg_pool_t *pool;
	ProcessPtr process;

	SpawnObject() {
		pool = psg_create_pool(PSG_DEFAULT_POOL_SIZE);
	}

	SpawnObject(BOOST_RV_REF(SpawnObject) r)
		: pool(r.pool),
		  process(r.process)
	{
		r.pool = NULL;
		r.process.reset();
	}

	~SpawnObject() {
		if (pool != NULL) {
			psg_destroy_pool(pool);
		}
	}

	SpawnObject &operator=(BOOST_RV_REF(SpawnObject) r) {
		if (this != &r) {
			if (pool != r.pool) {
				if (pool != NULL) {
					psg_destroy_pool(pool);
				}
				pool = r.pool;
			}
			r.pool = NULL;
			process = r.process;
			r.process.reset();
		}
		return *this;
	}
};


} // namespace ApplicationPool2
} // namespace Passenger

#endif /* _PASSENGER_APPLICATION_POOL_SPAWNER_OBJECT_H_ */
