/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#include <iostream>
#include <fstream>
#include "Logging.h"

namespace Passenger {

int _logLevel = 0;
ostream *_logStream = &cerr;

int
getLogLevel() {
	return _logLevel;
}

void
setLogLevel(int value) {
	_logLevel = value;
}

void
setDebugFile(const char *logFile) {
	#ifdef PASSENGER_DEBUG
		if (logFile != NULL) {
			ostream *stream = new ofstream(logFile, ios_base::out | ios_base::app);
			if (stream->fail()) {
				delete stream;
			} else {
				if (_logStream != NULL && _logStream != &cerr) {
					delete _logStream;
				}
				_logStream = stream;
			}
		} else {
			_logStream = &cerr;
		}
	#endif
}

} // namespace Passenger

