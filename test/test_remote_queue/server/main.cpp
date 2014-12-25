/*
 *  qianqians
 *  2014-10-5
 */
#include "../../third_party/json/json_protocol.h"
#include "../../remote-queue/remote_queue.h"

int main(){
	Fossilizid::remote_queue::QUEUE que = Fossilizid::remote_queue::queue();
	Fossilizid::remote_queue::ENDPOINT ep = Fossilizid::remote_queue::endpoint("127.0.0.1", 4567);
	Fossilizid::remote_queue::ACCEPTOR acceptor = Fossilizid::remote_queue::acceptor(que, ep);

	while (1){
		Fossilizid::remote_queue::EVENT ev = Fossilizid::remote_queue::queue(que);

		if (ev.type == Fossilizid::remote_queue::event_type_accept){
			Fossilizid::remote_queue::CHANNEL ch = Fossilizid::remote_queue::accept(ev.handle.acp);
			
			Json::Value value;
			value["test"] = "ok";
			Fossilizid::remote_queue::push(ch, value, Fossilizid::json_parser::json_to_buf);
		}
		else if (ev.type == Fossilizid::remote_queue::event_type_recv){
			Json::Value ret;
			if (Fossilizid::remote_queue::pop(ev.handle.ch, ret, Fossilizid::json_parser::buf_to_json)){
				printf("ret=%s\n", ret["ret"].asString().c_str());
				break;
			}
		}

		Sleep(1);
	}

	return 0;
}