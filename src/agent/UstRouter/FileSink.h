/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2015 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_UST_ROUTER_FILE_SINK_H_
#define _PASSENGER_UST_ROUTER_FILE_SINK_H_

#include <string>
#include <ctime>
#include <oxt/system_calls.hpp>
#include <Exceptions.h>
#include <FileDescriptor.h>
#include <UstRouter/LogSink.h>
#include <Utils/StrIntUtils.h>

namespace Passenger {
namespace UstRouter {

using namespace std;
using namespace oxt;


class FileSink: public LogSink {
public:
	string filename;
	FileDescriptor fd;

	FileSink(Controller *controller, const string &_filename)
		: LogSink(controller),
		  filename(_filename)
	{
		fd.assign(syscalls::open(_filename.c_str(),
			O_CREAT | O_WRONLY | O_APPEND,
			0600), __FILE__, __LINE__);
		if (fd == -1) {
			int e = errno;
			throw FileSystemException("Cannnot open file '" +
				filename + "' for appending", e, filename);
		}
	}

	virtual void append(const TransactionPtr &transaction) {
		StaticString data = transaction->getBody();
		LogSink::append(transaction);
		syscalls::write(fd, data.data(), data.size());
	}

	virtual Json::Value inspectStateAsJson() const {
		Json::Value doc = LogSink::inspectStateAsJson();
		doc["type"] = "file";
		doc["filename"] = filename;
		return doc;
	}

	string inspect() const {
		return "FileSink(" + filename + ")";
	}
};


} // namespace UstRouter
} // namespace Passenger

#endif /* _PASSENGER_UST_ROUTER_FILE_SINK_H_ */
