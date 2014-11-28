/*
 * connectservice.h
 *
 *  Created on: 2014-11-3
 *      Author: qianqians
 */
#ifndef _connectservice_h
#define _connectservice_h

#include "service.h"

namespace Fossilizid{
namespace reduce_rpc{

class connectservice : public service{
public:
	connectservice();
	~connectservice();
	
public:
	boost::shared_ptr<session> connect(char * ip, short port);

private:
	void run_network();

private:
	remote_queue::QUEUE que;

};

extern service * _service_handle;

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_service_h