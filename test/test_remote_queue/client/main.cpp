/*
 *  qianqians
 *  2014-10-5
 */
#include <algorithm>

#include "../../third_party/json/json_protocol.h"
#include "../../remote-queue/remote_queue.h"

int main(){
	(std::min)(1, 2);

	Fossilizid::remote_queue::ENDPOINT ep = Fossilizid::remote_queue::endpoint("127.0.0.1", 4567);
	Fossilizid::remote_queue::CHANNEL ch = Fossilizid::remote_queue::connect(ep);

	while (1){
		Json::Value ret;
		if (Fossilizid::remote_queue::pop(ch, ret, Fossilizid::json_parser::buf_to_json)){
			printf("test=%s\n", ret["test"].asString().c_str());

			Json::Value value;
			value["ret"] = "ok";
			Fossilizid::remote_queue::push(ch, value, Fossilizid::json_parser::json_to_buf);

			Sleep(1);

			break;
		}
	}

	return 0;
}