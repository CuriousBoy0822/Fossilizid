/*
 * json_protocol.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _json_protocol_h
#define _json_protocol_h

#include "jsoncpp/include/json/json.h"
#include "../../pool/factory.h"

#include <boost/bind.hpp>

namespace Fossilizid{
namespace json_parser{

inline boost::shared_ptr<std::string> json_to_buf(Json::Value & value){
	boost::shared_ptr<std::string> str = boost::shared_ptr<std::string>(pool::factory::create<std::string>(1), boost::bind(pool::factory::release<std::string>, _1, 1));

	Json::FastWriter write;
	*str = write.write(value);

	return str;
}

inline int buf_to_json(Json::Value & value, char * buf, int len){
	Json::Reader read;
	int begin = 0;

	while (buf[begin] != '[' && buf[begin] != '{'){
		begin++;

		if (begin >= len){
			return -1;
		}
	}

	char end = ' ';
	if (buf[begin] == '['){
		end = ']';
	}
	if (buf[begin] == '{'){
		end = '}';
	}

	int count = 1;
	char next = ' ';
	int i = begin;
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

		if (buf[i] == end && count == 1){
			count--;
			continue;
		}

		if (count == 0){
			break;
		}
	}

	if (count != 0){
		return -1;
	}

	read.parse(&buf[begin], &buf[i], value, false);

	return i;
}

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_json_protocol_h