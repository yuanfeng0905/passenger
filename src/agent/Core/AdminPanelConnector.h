/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2017 Phusion Holding B.V.
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
#ifndef _PASSENGER_ADMIN_PANEL_CONNECTOR_H_
#define _PASSENGER_ADMIN_PANEL_CONNECTOR_H_

#include <sys/wait.h>

#include <boost/bind.hpp>

#include <Constants.h>
#include <WebSocketCommandReverseServer.h>
#include <InstanceDirectory.h>
#include <Core/ApplicationPool/Pool.h>
#include <Utils/StrIntUtils.h>
#include <Utils/IOUtils.h>

#include <jsoncpp/json.h>

namespace Passenger {

using namespace std;
using namespace oxt;

class AdminPanelConnector {
public:
	class Schema: public WebSocketCommandReverseServer::Schema {
	private:
		static void validate(const ConfigKit::Store &config, vector<ConfigKit::Error> &errors) {
			using namespace ConfigKit;

			string integrationMode = config["integration_mode"].asString();

			if (integrationMode != "apache" && integrationMode != "nginx" && integrationMode != "standalone") {
				errors.push_back(Error("'{{integration_mode}}' must be one of 'apache', 'nginx' or 'standalone'"));
			}

			if (integrationMode == "standalone") {
				string engine = config["standalone_engine"].asString();
				if (engine != "" && engine != "nginx" && engine != "builtin") {
					errors.push_back(Error("'{{standalone_engine}}' must be one of 'nginx' or 'builtin'"));
				}
			}
		}

	public:
		Schema()
			: WebSocketCommandReverseServer::Schema(false)
		{
			using namespace ConfigKit;

			add("integration_mode", STRING_TYPE, REQUIRED);
			add("standalone_engine", STRING_TYPE, OPTIONAL);
			add("instance_dir", STRING_TYPE, OPTIONAL | READ_ONLY);
			add("web_server_module_version", STRING_TYPE, OPTIONAL | READ_ONLY);
			add("ruby", STRING_TYPE, OPTIONAL, "ruby");

			addValidator(validate);

			finalize();
		}
	};

	typedef WebSocketCommandReverseServer::ConnectionPtr ConnectionPtr;
	typedef WebSocketCommandReverseServer::MessagePtr MessagePtr;

private:
	WebSocketCommandReverseServer server;
	dynamic_thread_group threads;
	Json::Value globalPropertiesFromInstanceDir;

	bool onMessage(WebSocketCommandReverseServer *server,
		const ConnectionPtr &conn, const MessagePtr &msg)
	{
		Json::Value doc;

		try {
			doc = parseAndBasicValidateMessageAsJSON(msg->get_payload());
		} catch (const RuntimeException &e) {
			Json::Value reply;
			reply["result"] = "error";
			reply["request_id"] = doc["request_id"];
			reply["data"]["message"] = e.what();
			sendJsonReply(conn, reply);
			return true;
		}

		if (doc["action"] == "get") {
			return onGetMessage(conn, doc);
		} else {
			return onUnknownMessageAction(conn, doc);
		}
	}


	bool onGetMessage(const ConnectionPtr &conn, const Json::Value &doc) {
		const string resource = doc["resource"].asString();

		if (resource == "server_properties") {
			return onGetServerProperties(conn, doc);
		} else if (resource == "global_properties") {
			return onGetGlobalProperties(conn, doc);
		} else if (resource == "global_configuration") {
			return onGetGlobalConfiguration(conn, doc);
		} else if (resource == "global_statistics") {
			return onGetGlobalStatistics(conn, doc);
		} else if (resource == "application_properties") {
			return onGetApplicationProperties(conn, doc);
		} else if (resource == "application_config") {
			return onGetApplicationConfig(conn, doc);
		} else {
			return onUnknownResource(conn, doc);
		}
	}

	bool onGetServerProperties(const ConnectionPtr &conn, const Json::Value &doc) {
		threads.create_thread(
			boost::bind(&AdminPanelConnector::onGetServerPropertiesBgJob, this,
				conn, doc, server.getConfig()["ruby"].asString()),
			"AdminPanelCommandServer: get_server_properties background job",
			128 * 1024);
		return false;
	}

	void onGetServerPropertiesBgJob(const ConnectionPtr &conn, const Json::Value &doc,
		const string &ruby)
	{
		vector<string> args;
		args.push_back("passenger-config");
		args.push_back("system-properties");

		int status = 0;
		string output;
		try {
			output = runInternalRubyTool(*resourceLocator, ruby, args, &status);
		} catch (const std::exception &e) {
			server.getIoService().post(boost::bind(
				&AdminPanelConnector::onGetServerPropertiesDone, this,
				conn, doc, string(), -1, e.what()
			));
			return;
		}

		server.getIoService().post(boost::bind(
			&AdminPanelConnector::onGetServerPropertiesDone, this,
			conn, doc, output, status, string()
		));
	}

	void onGetServerPropertiesDone(const ConnectionPtr &conn, const Json::Value &doc,
		const string output, int status, const string &error)
	{
		Json::Value reply;
		reply["request_id"] = doc["request_id"];
		if (error.empty()) {
			if (status == 0 || status == -1) {
				Json::Reader reader;
				Json::Value dataDoc;

				if (output.empty()) {
					reply["result"] = "error";
					reply["data"]["message"] = "Error parsing internal helper tool output";
					P_ERROR(getLogPrefix() << "Error parsing internal helper tool output.\n" <<
						"Raw data: \"\"");
				} else if (reader.parse(output, dataDoc)) {
					reply["result"] = "ok";
					reply["data"] = dataDoc;
				} else {
					reply["result"] = "error";
					reply["data"]["message"] = "Error parsing internal helper tool output";
					P_ERROR(getLogPrefix() << "Error parsing internal helper tool output.\n" <<
						"Error: " << reader.getFormattedErrorMessages() << "\n"
						"Raw data: \"" << cEscapeString(output) << "\"");
				}
			} else {
				int exitStatus = WEXITSTATUS(status);
				reply["result"] = "error";
				reply["data"]["message"] = "Internal helper tool exited with status "
					+ toString(exitStatus);
				P_ERROR(getLogPrefix() << "Internal helper tool exited with status "
					<< exitStatus << ". Raw output: \"" << cEscapeString(output) << "\"");
			}
		} else {
			reply["result"] = "error";
			reply["data"]["message"] = error;
		}
		sendJsonReply(conn, reply);
		server.doneReplying(conn);
	}

	bool onGetGlobalProperties(const ConnectionPtr &conn, const Json::Value &doc) {
		Json::Value reply, data;
		reply["result"] = "ok";
		reply["request_id"] = doc["request_id"];

		data = globalPropertiesFromInstanceDir;
		data["version"] = PASSENGER_VERSION;
		data["core_pid"] = Json::UInt(getpid());

		const ConfigKit::Store &config = server.getConfig();
		string integrationMode = config["integration_mode"].asString();
		data["integration_mode"]["name"] = integrationMode;
		if (!config["web_server_module_version"].isNull()) {
			data["integration_mode"]["server_module_version"] = config["server_module_version"];
		}
		if (integrationMode == "standalone") {
			data["integration_mode"]["standalone_engine"] = config["standalone_engine"];
		}

		reply["data"] = data;
		sendJsonReply(conn, reply);
		return true;
	}

	bool onGetGlobalConfiguration(const ConnectionPtr &conn, const Json::Value &doc) {
		Json::Value reply;
		reply["result"] = "error";
		reply["request_id"] = doc["request_id"];
		reply["data"]["message"] = "Action not implemented";
		sendJsonReply(conn, reply);
		return true;
	}

	bool onGetGlobalStatistics(const ConnectionPtr &conn, const Json::Value &doc) {
		Json::Value reply;
		reply["result"] = "error";
		reply["request_id"] = doc["request_id"];
		reply["data"]["message"] = "Action not implemented";
		sendJsonReply(conn, reply);
		return true;
	}

	bool onGetApplicationProperties(const ConnectionPtr &conn, const Json::Value &doc) {
		Json::Value reply;
		reply["result"] = "error";
		reply["request_id"] = doc["request_id"];
		reply["data"]["message"] = "Action not implemented";
		sendJsonReply(conn, reply);
		return true;
	}

	bool onGetApplicationConfig(const ConnectionPtr &conn, const Json::Value &doc) {
		Json::Value reply;
		reply["result"] = "error";
		reply["request_id"] = doc["request_id"];
		reply["data"]["message"] = "Action not implemented";
		sendJsonReply(conn, reply);
		return true;
	}


	bool onUnknownResource(const ConnectionPtr &conn, const Json::Value &doc) {
		Json::Value reply;
		reply["result"] = "error";
		reply["request_id"] = doc["request_id"];
		reply["data"]["message"] = "Unknown resource '" + doc["resource"].asString() + "'";
		sendJsonReply(conn, reply);
		return true;
	}

	bool onUnknownMessageAction(const ConnectionPtr &conn, const Json::Value &doc) {
		Json::Value reply;
		reply["result"] = "error";
		reply["request_id"] = doc["request_id"];
		reply["data"]["message"] = "Unknown action '" + doc["action"].asString() + "'";
		sendJsonReply(conn, reply);
		return true;
	}


	Json::Value parseAndBasicValidateMessageAsJSON(const string &msg) const {
		Json::Value doc;
		Json::Reader reader;
		if (!reader.parse(msg, doc)) {
			throw RuntimeException("Error parsing command JSON document: "
				+ reader.getFormattedErrorMessages());
		}

		if (!doc.isObject()) {
			throw RuntimeException("Invalid command JSON document: must be an object");
		}
		if (!doc.isMember("action")) {
			throw RuntimeException("Invalid command JSON document: missing 'action' key");
		}
		if (!doc["action"].isString()) {
			throw RuntimeException("Invalid command JSON document: 'action' key must be a string");
		}
		if (!doc.isMember("request_id")) {
			throw RuntimeException("Invalid command JSON document: missing 'request_id' key");
		}
		if (!doc.isMember("resource")) {
			throw RuntimeException("Invalid command JSON document: missing 'resource' key");
		}
		if (!doc["resource"].isString()) {
			throw RuntimeException("Invalid command JSON document: 'resource' key must be a string");
		}

		return doc;
	}

	void sendJsonReply(const ConnectionPtr &conn, const Json::Value &doc) {
		Json::FastWriter writer;
		string str = writer.write(doc);
		conn->send(str);
	}

	void readInstanceDirProperties(const string &instanceDir) {
		Json::Value doc;
		Json::Reader reader;

		if (!reader.parse(readAll(instanceDir + "/properties.json"), doc)) {
			throw RuntimeException("Cannot parse " + instanceDir + "/properties.json: "
				+ reader.getFormattedErrorMessages());
		}

		globalPropertiesFromInstanceDir["instance_id"] = doc["instance_id"];
		globalPropertiesFromInstanceDir["watchdog_pid"] = doc["watchdog_pid"];
	}

	void initializePropertiesWithoutInstanceDir() {
		globalPropertiesFromInstanceDir["instance_id"] =
			InstanceDirectory::generateInstanceId();
	}

	string getLogPrefix() const {
		return server.getConfig()["log_prefix"].asString();
	}

	WebSocketCommandReverseServer::MessageHandler createMessageFunctor() {
		return boost::bind(&AdminPanelConnector::onMessage, this,
			boost::placeholders::_1, boost::placeholders::_2,
			boost::placeholders::_3);
	}

public:
	/******* Dependencies *******/

	ResourceLocator *resourceLocator;
	ApplicationPool2::PoolPtr appPool;


	AdminPanelConnector(const Schema &schema, const Json::Value &config)
		: server(schema, createMessageFunctor(), config),
		  resourceLocator(NULL)
	{
		if (!config["instance_dir"].isNull()) {
			readInstanceDirProperties(config["instance_dir"].asString());
		} else {
			initializePropertiesWithoutInstanceDir();
		}
	}

	void initialize() {
		if (resourceLocator == NULL) {
			throw RuntimeException("resourceLocator must be non-NULL");
		}
		if (appPool == NULL) {
			throw RuntimeException("appPool must be non-NULL");
		}
		server.initialize();
	}

	void run() {
		server.run();
	}

	void asyncShutdown(const WebSocketCommandReverseServer::Callback &callback
		= WebSocketCommandReverseServer::Callback())
	{
		server.asyncShutdown(callback);
	}
};

} // namespace Passenger

#endif /* _PASSENGER_ADMIN_PANEL_CONNECTOR_H_ */
