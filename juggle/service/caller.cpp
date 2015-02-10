/*
 * caller.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "caller.h"
#include "service.h"
#include "globalhandle.h"

#include "../interface/semaphore.h"

namespace Fossilizid{
namespace juggle {

caller::caller(channel * ch, std::string modulename){
	_module_name = modulename;
	_ch = ch;
}

caller::~caller(){
}

std::string caller::module_name(){
	return _module_name;
}

uuid::uuid caller::module_id(){
	return _module_id;
}

boost::shared_ptr<object> caller::call_module_method_sync(std::string methodname, boost::shared_ptr<object> value){
	semaphore s;
	call_module_method_async(methodname, value, boost::bind(&semaphore::post, &s, _1));
	return s.wait();
}

void caller::call_module_method_async(std::string methodname, boost::shared_ptr<object> value, boost::function<void(boost::shared_ptr<object>)> callback){
	(*value)["method"] = methodname;
	(*value)["suuid"] = uuid::UUID();
	(*value)["rpcevent"] = "call_rpc_method";

	boost::static_pointer_cast<juggleservice>(_service_handle)->register_rpc_callback((*value)["suuid"].asstring(), callback);
	
	_ch->push(value);
}

} /* namespace juggle */
} /* namespace Fossilizid */