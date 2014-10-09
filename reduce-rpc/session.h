/*
 * session.h
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#ifndef _session_h
#define _session_h

#include "../remote-queue/queue.h"
#include "../../jsoncpp/jsoncpp/include/json/json.h"

namespace Fossilizid{
namespace reduce_rpc{

class session{
public:
	session();
	~session();

	void call_rpc_mothed();

private:
	remote_queue::CHANNEL ch;
	

};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_session_h