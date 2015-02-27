/*
 * module.cpp
 *
 *  Created on: 2015-1-14
 *      Author: qianqians
 */
#include "module.h"

namespace Fossilizid{
namespace juggle {

module::module(std::string modulename, uuid::uuid & moduleid){
	_module_name = modulename;
	_module_id = moduleid;
}

module::~module(){
}

std::string module::module_name(){
	return _module_name;
}

uuid::uuid module::module_id(){
	return _module_id;
}

void module::call_module_method(boost::shared_ptr<channel> ch, boost::shared_ptr<object> value){
}

} /* namespace juggle */
} /* namespace Fossilizid */