/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2014 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#include <DataStructures/HashedStaticString.h>

namespace Passenger {
namespace ServerKit {


extern const HashedStaticString TRANSFER_ENCODING;
extern const char DEFAULT_INTERNAL_SERVER_ERROR_RESPONSE[];
extern const unsigned int DEFAULT_INTERNAL_SERVER_ERROR_RESPONSE_SIZE;

const char DEFAULT_INTERNAL_SERVER_ERROR_RESPONSE[] =
	"Status: 500 Internal Server Error\r\n"
	"Content-Length: 22\r\n"
	"Content-Type: text/plain\r\n"
	"Connection: close\r\n"
	"\r\n"
	"Internal server error\n";
const unsigned int DEFAULT_INTERNAL_SERVER_ERROR_RESPONSE_SIZE =
	sizeof(DEFAULT_INTERNAL_SERVER_ERROR_RESPONSE) - 1;


} // namespace ServerKit
} // namespace
