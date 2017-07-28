/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014-2017 Phusion Holding B.V.
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
#ifndef _PASSENGER_SERVER_KIT_CONTEXT_H_
#define _PASSENGER_SERVER_KIT_CONTEXT_H_

#include <boost/make_shared.hpp>
#include <string>
#include <cstddef>
#include <MemoryKit/mbuf.h>
#include <ConfigKit/ConfigKit.h>
#include <ConfigKit/PrefixTranslator.h>
#include <SafeLibev.h>
#include <Constants.h>
#include <Utils.h>
#include <Utils/StrIntUtils.h>
#include <Utils/JsonUtils.h>

extern "C" {
	struct uv_loop_s;
}

namespace Passenger {
namespace ServerKit {


struct FileBufferedChannelConfig {
private:
	ConfigKit::Store store;

	void updateConfigCache() {
		bufferDir = store["buffer_dir"].asString();
		threshold = store["threshold"].asUInt();
		delayInFileModeSwitching = store["delay_in_file_mode_switching"].asUInt();
		maxDiskChunkReadSize = store["max_disk_chunk_read_size"].asUInt();
		autoTruncateFile = store["auto_truncate_file"].asBool();
		autoStartMover = store["auto_start_mover"].asBool();
	}

public:
	struct Schema: public ConfigKit::Schema {
		Schema() {
			using namespace ConfigKit;

			add("buffer_dir", STRING_TYPE, OPTIONAL, getSystemTempDir());
			add("threshold", UINT_TYPE, OPTIONAL, DEFAULT_FILE_BUFFERED_CHANNEL_THRESHOLD);
			add("delay_in_file_mode_switching", UINT_TYPE, OPTIONAL, 0);
			add("max_disk_chunk_read_size", UINT_TYPE, OPTIONAL, 0);
			add("auto_truncate_file", BOOL_TYPE, true);
			add("auto_start_mover", BOOL_TYPE, true); // For unit testing purposes

			finalize();
		}
	};

	// Cached config values. Do not update directly; use the mutation
	// methods instead.
	string bufferDir;
	unsigned int threshold;
	unsigned int delayInFileModeSwitching;
	unsigned int maxDiskChunkReadSize;
	bool autoTruncateFile;
	bool autoStartMover;

	FileBufferedChannelConfig(const Schema &schema,
		const Json::Value &initialConfig = Json::Value())
		: store(schema)
	{
		vector<ConfigKit::Error> errors;

		if (!store.update(initialConfig, errors)) {
			throw ArgumentException("Invalid initial configuration: "
				+ toString(errors));
		}
		updateConfigCache();
	}

	Json::Value previewConfigUpdate(const Json::Value &updates,
		vector<ConfigKit::Error> &errors)
	{
		return store.previewUpdate(updates, errors);
	}

	bool configure(const Json::Value &updates, vector<ConfigKit::Error> &errors) {
		if (store.update(updates, errors)) {
			updateConfigCache();
			return true;
		} else {
			return false;
		}
	}

	Json::Value inspectConfig() const {
		return store.inspect();
	}
};

class Context {
private:
	ConfigKit::Store config;

	void updateConfigCache() {
		secureModePassword = config["secure_mode_password"].asString();
	}

public:
	struct Schema: public ConfigKit::Schema {
		struct FileBufferedChannelConfigSubschemaStruct {
			FileBufferedChannelConfig::Schema schema;
			ConfigKit::PrefixTranslator translator;

			FileBufferedChannelConfigSubschemaStruct()
				: translator("file_buffered_channel_")
				{ }
		} fileBufferedChannelConfig;

		Schema() {
			using namespace ConfigKit;

			addSubSchema(fileBufferedChannelConfig.schema);

			add("mbuf_block_chunk_size", UINT_TYPE, OPTIONAL | READ_ONLY,
				DEFAULT_MBUF_CHUNK_SIZE);
			add("secure_mode_password", PASSWORD_TYPE, OPTIONAL);

			finalize();
		}
	};

	// Dependencies
	SafeLibevPtr libev;
	struct uv_loop_s *libuv;

	// Subcomponents
	struct MemoryKit::mbuf_pool mbuf_pool;
	FileBufferedChannelConfig fileBufferedChannelConfig;

	// Cached config values. Do not update directly; use the mutation
	// methods instead.
	string secureModePassword;

	Context(const Schema &schema, const Json::Value &initialConfig = Json::Value())
		: config(schema),
		  libuv(NULL),
		  fileBufferedChannelConfig(schema.fileBufferedChannelConfig.schema, initialConfig)
	{
		vector<ConfigKit::Error> errors;

		if (!config.update(initialConfig, errors)) {
			throw ArgumentException("Invalid initial configuration: "
				+ toString(errors));
		}
		updateConfigCache();
	}

	~Context() {
		MemoryKit::mbuf_pool_deinit(&mbuf_pool);
	}

	void initialize() {
		if (libev == NULL) {
			throw RuntimeException("libev must be non-NULL");
		}
		if (libuv == NULL) {
			throw RuntimeException("libuv must be non-NULL");
		}

		mbuf_pool.mbuf_block_chunk_size = config["mbuf_block_chunk_size"].asUInt();
		MemoryKit::mbuf_pool_init(&mbuf_pool);
	}

	Json::Value previewConfigUpdate(const Json::Value &updates,
		vector<ConfigKit::Error> &errors)
	{
		using namespace ConfigKit;
		const Schema &schema = static_cast<const Schema &>(config.getSchema());

		previewConfigUpdateSubComponent(fileBufferedChannelConfig, updates,
			schema.fileBufferedChannelConfig.translator, errors);
		return config.previewUpdate(updates, errors);
	}

	bool configure(const Json::Value &updates, vector<ConfigKit::Error> &errors) {
		using namespace ConfigKit;
		const Schema &schema = static_cast<const Schema &>(config.getSchema());

		previewConfigUpdate(updates, errors);

		if (errors.empty()) {
			configureSubComponent(fileBufferedChannelConfig, updates,
				schema.fileBufferedChannelConfig.translator, errors);
			config.update(updates, errors);
			updateConfigCache();
		}

		return errors.empty();
	}

	Json::Value inspectConfig() const {
		return config.inspect();
	}

	Json::Value inspectStateAsJson() const {
		Json::Value doc;
		Json::Value mbufDoc;

		mbufDoc["free_blocks"] = (Json::UInt) mbuf_pool.nfree_mbuf_blockq;
		mbufDoc["active_blocks"] = (Json::UInt) mbuf_pool.nactive_mbuf_blockq;
		mbufDoc["chunk_size"] = (Json::UInt) mbuf_pool.mbuf_block_chunk_size;
		mbufDoc["offset"] = (Json::UInt) mbuf_pool.mbuf_block_offset;
		mbufDoc["spare_memory"] = byteSizeToJson(mbuf_pool.nfree_mbuf_blockq
			* mbuf_pool.mbuf_block_chunk_size);
		mbufDoc["active_memory"] = byteSizeToJson(mbuf_pool.nactive_mbuf_blockq
			* mbuf_pool.mbuf_block_chunk_size);
		#ifdef MBUF_ENABLE_DEBUGGING
			struct MemoryKit::active_mbuf_block_list *list =
				const_cast<struct MemoryKit::active_mbuf_block_list *>(
					&mbuf_pool.active_mbuf_blockq);
			struct MemoryKit::mbuf_block *block;
			Json::Value listJson(Json::arrayValue);

			TAILQ_FOREACH (block, list, active_q) {
				Json::Value blockJson;
				blockJson["refcount"] = block->refcount;
				#ifdef MBUF_ENABLE_BACKTRACES
					blockJson["backtrace"] =
						(block->backtrace == NULL)
						? "(null)"
						: block->backtrace;
				#endif
				listJson.append(blockJson);
			}
			mbufDoc["active_blocks_list"] = listJson;
		#endif

		doc["mbuf_pool"] = mbufDoc;

		return doc;
	}
};


} // namespace ServerKit
} // namespace Passenger

#endif /* _PASSENGER_SERVER_KIT_CONTEXT_H_ */
