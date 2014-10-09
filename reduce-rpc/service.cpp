/*
 * service.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "service.h"

namespace Fossilizid{
namespace reduce_rpc {

service::service(char * ip, short port){
	isrun.store(true);

	clockstamp = _clock();
	timestamp = time(0) + clockstamp;

	ep = remote_queue::endpoint(ip, port);

	que = remote_queue::queue();

	acp = remote_queue::acceptor(que, ep);

	_thread_group.create_thread(boost::bind(&service::run, this));
}

service::~service(){
	isrun.store(false);

	_thread_group.join_all();

	remote_queue::close(que);

	for (auto v : map_session){
		remote_queue::close(v.first);
	}
	map_session.clear();
}

void service::run(){
	while (isrun.load()){
		remote_queue::EVENT ev = remote_queue::queue(que);
		switch (ev.type)
		{
		case remote_queue::event_type_none:
			break;
				
		case remote_queue::event_type_accept:
			ev.handle.acp;
			break;

		case remote_queue::event_type_recv:
			break;

		case remote_queue::event_type_disconnect:
			break;

		default:
			break;
		}

		timestamp += _clock() - clockstamp;

	}
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */