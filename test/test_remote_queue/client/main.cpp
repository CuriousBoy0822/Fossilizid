/*
 *  qianqians
 *  2014-10-5
 */
#include <algorithm>

#include "../../third_party/json/json_protocol.h"
#include "../../remoteq/remote_queue.h"

int main(){
	Fossilizid::remoteq::ENDPOINT ep = Fossilizid::remoteq::endpoint("127.0.0.1", 4567);
	Fossilizid::remoteq::CHANNEL ch = Fossilizid::remoteq::connect(ep);

	while (1){
		Json::Value ret;
		if (Fossilizid::remoteq::pop(ch, ret, Fossilizid::json_parser::buf_to_json)){
			printf("test=%s\n", ret["test"].asString().c_str());

			Json::Value value;
			value["ret"] = "ok";
			Fossilizid::remoteq::push(ch, value, Fossilizid::json_parser::json_to_buf);

			Sleep(1);

			break;
		}
	}

	return 0;
}