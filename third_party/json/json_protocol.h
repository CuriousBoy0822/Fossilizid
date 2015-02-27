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

#include <list>

namespace Fossilizid{
namespace json_parser{

inline boost::shared_ptr<std::string> json_to_buf(Json::Value & value){
	boost::shared_ptr<std::string> str = boost::shared_ptr<std::string>(pool::factory::create<std::string>(1), boost::bind(pool::factory::release<std::string>, _1, 1));

	Json::FastWriter write;
	*str = write.write(value);

	size_t begin = 0;
	while ((*str)[begin] != '[' && (*str)[begin] != '{'){
		begin++;

		if (begin >= (*str).size()){
			throw std::exception("error format json string");
		}
	}

	char end = ' ';
	if ((*str)[begin] == '['){
		end = ']';
	}
	if ((*str)[begin] == '{'){
		end = '}';
	}

	auto endjson = str->back();
	while(endjson != end){
		str->pop_back();
		endjson = str->back();
	}

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

	std::list<char> next;
	int i = begin;
	for (; i < len; i++){
		if (buf[i] == '['){
			next.push_back(']');
		}
		if (buf[i] == '{'){
			next.push_back('}');
		}

		if (buf[i] == next.back()){
			next.pop_back();
			continue;
		}

		if (buf[i] == end && next.size() == 1){
			next.pop_back();
			continue;
		}

		if (next.size() == 0){
			break;
		}
	}

	if (next.size() != 0){
		return -1;
	}

	read.parse(&buf[begin], &buf[i], value, false);

	return i;
}

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_json_protocol_h