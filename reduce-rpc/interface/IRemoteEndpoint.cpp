/*
 * IRemoteEndpoint.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "IRemoteEndpoint.h"
#include "../service/connectservice.h"
#include "reduce-rpc\service\session.h"

namespace Fossilizid{
namespace reduce_rpc {

void IRemoteEndpoint::ConnectService(char * ip, short port){
	boost::shared_ptr<session> _session = _connectservice.connect(ip, port);

	Json::Value value;
	value["suuid"] = UUID();
	value["epuuid"] = UUID();
	value["eventtype"] = "connect_server";
	_session->do_push(_session, value);
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */