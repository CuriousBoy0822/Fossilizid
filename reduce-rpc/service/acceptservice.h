/*
 * acceptservice.h
 *
 *  Created on: 2014-11-3
 *      Author: qianqians
 */
#ifndef _acceptservice_h
#define _acceptservice_h

#include "service.h"

namespace Fossilizid{
namespace reduce_rpc{

class acceptservice : public service{
public:
	acceptservice(char * ip, short port);
	~acceptservice();

private:
	void run_network();

private:
	remote_queue::ENDPOINT ep;
	remote_queue::QUEUE que;
	remote_queue::ACCEPTOR acp;

};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_acceptservice_h