/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2017 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#ifndef _PASSENGER_CORE_SCHEMA_H_
#define _PASSENGER_CORE_SCHEMA_H_

#include <string>
#include <vector>

#include <ConfigKit/Schema.h>
#include <ServerKit/HttpServer.h>
#include <Core/Controller/Config.h>
#include <Core/ApiServer.h>

namespace Passenger {
namespace Core {

// You can view the full schema by running:
// ./buildout/support-binaries/PassengerAgent print-core-config-schema
class Schema: public ConfigKit::Schema {
private:
	// Prefix config options that come from ServerKit
	static void addHttpServerSubSchemaTranslations(ConfigKit::TableTranslator &translator,
		const StaticString &prefix)
	{
		vector<string> keys = ServerKit::HttpServerSchema().inspect().getMemberNames();
		vector<string>::const_iterator it, end = keys.end();
		for (it = keys.begin(); it != end; it++) {
			translator.add(prefix + *it, *it);
		}
	}

public:
	struct {
		ControllerSchema schema;
		ConfigKit::TableTranslator translator;
	} controller;
	struct {
		ApiServer::Schema schema;
		ConfigKit::TableTranslator translator;
	} apiServer;

	Schema() {
		using namespace ConfigKit;

		// Add subschema: controller
		addHttpServerSubSchemaTranslations(controller.translator, "core_controller_");
		addSubSchema(controller.schema, controller.translator);
		remove("thread_number"); // Per-thread option

		// Add subschema: API server
		apiServer.translator.add("watchdog_fd_passing_password", "fd_passing_password");
		addHttpServerSubSchemaTranslations(apiServer.translator, "core_api_server_");
		addSubSchema(apiServer.schema, apiServer.translator);

		override("instance_dir", STRING_TYPE, REQUIRED | READ_ONLY);

		//core_password
		//core_password_file

		finalize();
	}
};

} // namespace Core
} // namespace Passenger

#endif /* _PASSENGER_CORE_SCHEMA_H_ */
