/*
 * rpcsession.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "service.h"
#include "rpcsession.h"
#include "obj.h"
#include "remote_obj.h"

#include "../../third_party/json/json_protocol.h"

namespace Fossilizid{
namespace reduce_rpc {

rpcsession::rpcsession(uuid epuuid, remote_queue::CHANNEL _ch){
	ch = _ch;
	_epuuid = epuuid;
}

rpcsession::~rpcsession(){
	mapremoteobj.clear();
}

void rpcsession::reset(remote_queue::CHANNEL _ch){
	ch = _ch;
}

void rpcsession::register_global_obj(boost::shared_ptr<obj> obj){
	boost::mutex::scoped_lock lock(mu_mapremoteobj);
	mapremoteobj.insert(std::make_pair(obj->objid(), obj));
	mapremoteobj_classname.insert(std::make_pair(obj->class_name(), obj));
}

boost::shared_ptr<obj> rpcsession::get_global_obj(std::string classname){
	boost::mutex::scoped_lock lock(mu_mapremoteobj);
	auto find = mapremoteobj.find(classname);
	if (find != mapremoteobj_classname.end()){
		return find->second;
	}
	return nullptr;
}

void rpcsession::global_obj_lock(){
	mu_mapremoteobj.lock();
}

void rpcsession::global_obj_unlock(){
	mu_mapremoteobj.unlock();
}

rpcsession::global_obj_iterator rpcsession::global_obj_begin(){
	return mapremoteobj.begin();
}

service::global_obj_iterator rpcsession::global_obj_end(){
	return mapremoteobj.end();
}

void rpcsession::do_time(boost::uint64_t time){
	for (auto obj : mapremoteobj){
		obj.second->call_do_time(time);
	}
}

void rpcsession::do_pop(boost::shared_ptr<session> session, Json::Value & value){
	Json::Value eventtype = value.get("eventtype", Json::nullValue);
	if (eventtype.isNull()){
		return;
	}
	
	if (eventtype.asString() == "rpc_event"){
		Json::Value _uuid = value.get("objuuid", Json::nullValue);
		if (_uuid.isNull()){
			return;
		}

		std::unordered_map<uuid, boost::shared_ptr<obj> > ::iterator it = mapremoteobj.find(_uuid.asString());
		if (it != mapremoteobj.end()){
			if (it->second != 0){
				it->second->push_rpc_event(std::make_pair(session, value));
			}
		}
	} else if (eventtype.asString() == "create_obj"){
		Json::Value _uuid = value.get("objuuid", Json::nullValue);
		if (_uuid.isNull()){
			return;
		}

		mapremoteobj.insert(std::make_pair(_uuid.asString(), boost::shared_ptr<obj>(new remote_obj(session, value))));
	}
}

bool rpcsession::do_push(boost::shared_ptr<session> session, Json::Value & value){
	if (session != 0){
		return remote_queue::push(static_cast<rpcsession*>(session.get())->ch, value, boost::bind(json_parser::json_to_buf, _1));
	}
	return false;
}

void rpcsession::do_logic(){
	for (auto var : mapremoteobj){
		_service_handle->push_current_obj(var.second);

		var.second->call_do_logic();

		_service_handle->pop_current_obj();
	}
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */