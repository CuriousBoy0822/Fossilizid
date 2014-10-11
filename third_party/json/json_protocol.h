/*
 * json_protocol.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _json_protocol_h
#define _json_protocol_h

#include "jsoncpp/include/json/json.h"
#include "../../pool/mempool.h"

namespace Fossilizid{
namespace json_parser{

std::pair<char *, int> json_to_buf(Json::Value & value){
	Json::FastWriter write;
	std::string str = write.write(value);

	char * buf = (char*)pool::mempool::allocator(str.size());
	memcpy(buf, str.c_str(), str.size());

	return std::make_pair(buf, str.size());
}

int buf_to_json(Json::Value & value, char * buf, int len){
	Json::Reader read;
	if (buf[0] != '[' && buf[0] != '{'){
		return -1;
	}

	char end = ' ';
	if (buf[0] != '['){
		end = ']';
	}
	if (buf[0] != '{'){
		end = '}';
	}

	int count = 1;
	char next = ' ';
	int i = 1;
	for (; i < len; i++){
		if (buf[i] == '['){
			next = ']';
			count++;
		}
		if (buf[i] == '{'){
			next = '}';
			count++;
		}

		if (buf[i] == next){
			count--;
		}

		if (buf[i] != end && count == 1){
			count--;
			break;
		}
	}

	if (count != 0){
		return false;
	}

	read.parse(&buf[0], &buf[i], value);

	return i+1;
}

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_json_protocol_h