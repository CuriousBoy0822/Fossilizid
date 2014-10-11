/*
 * service.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "service.h"
#include "../third_party/json/json_protocol.h"

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
			{
				remote_queue::CHANNEL ch = remote_queue::accept(ev.handle.acp);
				if (ch != 0){
					map_session[ch] = boost::shared_ptr<session>(new session(ch));
				}
			}
			break;

		case remote_queue::event_type_recv:
			{
				remote_queue::CHANNEL ch = ev.handle.ch;
				Json::Value value;
				if (remote_queue::pop(ch, value, boost::bind(json_parser::buf_to_json))){
					map_session[ch]->do_recv(value);
				}
			}
			break;

		case remote_queue::event_type_disconnect:
			{
				map_session.erase(ev.handle.ch);
				remote_queue::close(ev.handle.ch);
			}
			break;

		default:
			break;
		}

		timestamp += _clock() - clockstamp;
		for (auto var : map_session){
			var.second->do_time(timestamp);
		}
	}
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */