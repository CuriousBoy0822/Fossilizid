/*
 * acceptservice.cpp
 *
 *  Created on: 2014-11-3
 *      Author: qianqians
 */
#include "acceptservice.h"
#include "rpcsession.h"
#include "tempsession.h"
#include "obj.h"

#include "../../third_party/json/json_protocol.h"

namespace Fossilizid{
namespace reduce_rpc {

acceptservice::acceptservice(char * ip, short port) : service(){
	ep = remote_queue::endpoint(ip, port);
	que = remote_queue::queue();
	acp = remote_queue::acceptor(que, ep);
}

acceptservice::~acceptservice(){
	remote_queue::close(que);
}

void acceptservice::run_network(){
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
					boost::unique_lock<boost::shared_mutex> lock(mu_map_session);
					map_session[ch] = boost::shared_ptr<session>(new tempsession(ch, "acceptservice"));
				}
			}
			break;

		case remote_queue::event_type_recv:
			{	
				remote_queue::CHANNEL ch = ev.handle.ch;
				
				Json::Value value;
				while (remote_queue::pop(ch, value, json_parser::buf_to_json)){
					Json::Value _suuid = value.get("suuid", Json::nullValue);
					if (_suuid.isNull()){
						continue;
					}

					{
						boost::shared_lock<boost::shared_mutex> lock(mu_map_session);
						map_session[ch]->do_pop(map_session[ch], value);
					}

					boost::mutex::scoped_lock lock(mu_wait_context_list);
					auto finduuidcontex = wait_context_list.find(_suuid.asString());
					if (finduuidcontex != wait_context_list.end()){
						boost::mutex::scoped_lock lock(mu_wake_up_set);
						wake_up_set.insert(_suuid.asString());
					}
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
	}
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */