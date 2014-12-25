/*
 * IRemoteEndpoint.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include <boost/atomic.hpp>

#include "IRemoteEndpoint.h"

namespace Fossilizid{
namespace reduce_rpc {

static connectservice _connectservice;
static boost::atomic_bool isinit(false);

IRemoteEndpoint ConnectService(char * ip, short port){
	if (isinit.exchange(true) == false){
		_connectservice.init();
	}
	
	IRemoteEndpoint ep;

	ep._session = _connectservice.connect(ip, port);

	Json::Value value;
	value["suuid"] = UUID();
	value["epuuid"] = UUID();
	value["eventtype"] = "connect_server";
	value["globalobjarray"] = Json::Value(Json::arrayValue);
	_service_handle->global_obj_lock();
	for (service::global_obj_iterator it = _service_handle->global_obj_begin(); it != _service_handle->global_obj_end(); it++){
		Json::Value objinfo;
		objinfo["classname"] = it->second->class_name();
		objinfo["objid"] = it->second->objid();
		value["globalobjarray"].append(objinfo);
	}
	_service_handle->global_obj_unlock();

	ep._session->do_push(ep._session, value);

	_service_handle->wait(value["suuid"].asString(), 15);

	ep._session = _connectservice.get_rpcsession(value["epuuid"].asString());

	return ep;
}

boost::shared_ptr<session> GetSession(IRemoteEndpoint ep){
	return ep._session;
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */