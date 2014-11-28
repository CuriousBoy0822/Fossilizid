/*
 * ILocaleEndpoint.h
 *
 *  Created on: 2014-10-30
 *      Author: qianqians
 */
#ifndef _ILocaleEndpoint_h
#define _ILocaleEndpoint_h

#include "../service/acceptservice.h"

namespace Fossilizid{
namespace reduce_rpc{

class ILocaleEndpoint{
public:
	ILocaleEndpoint(char * ip, short port) : _service(ip, port){
		_service.init();
	}

private:
	acceptservice _service;

};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_ILocaleEndpoint_h