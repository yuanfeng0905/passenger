/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_CLASS_UTILS_H_
#define _PASSENGER_CLASS_UTILS_H_

#define P_DEFINE_GETTER_CONST_REF(type, name) \
	const type &get ## name() const { \
		return m ## name; \
	}

#define P_DEFINE_GETTER_REF(type, name) \
	type &get ## name() { \
		return m ## name; \
	}

#define P_DEFINE_SETTER(type, name) \
	void set ## name(const type &value) { \
		m ## name = value; \
	}

#define P_PROPERTY_CONST_REF(visibility, type, name) \
	public: \
		P_DEFINE_GETTER_CONST_REF(type, name) \
		P_DEFINE_SETTER(type, name) \
	visibility: \
		type m ## name

#define P_RO_PROPERTY_REF(visibility, type, name) \
	public: \
		P_DEFINE_GETTER_REF(type, name) \
	visibility: \
		type m ## name

#endif /* _PASSENGER_CLASS_UTILS_H_ */
