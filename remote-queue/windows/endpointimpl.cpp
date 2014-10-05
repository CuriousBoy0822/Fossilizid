/*
 * endpointimpl.cpp
 *
 *  Created on: 2014-8-20
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <Windows.h>

#include "../../pool/objpool.h"

#include "endpointimpl.h"

namespace Fossilizid{
namespace remote_queue {

ENDPOINT endpoint(char * ip, short port){
	endpointimpl * impl = pool::objpool<endpointimpl>::allocator(1);

	impl->addr.sin_family = AF_INET;
	impl->addr.sin_addr.S_un.S_addr = inet_addr(ip);
	impl->addr.sin_port = port;
	impl->_handle_type = handle_endpoint_type;

	return (ENDPOINT)impl;
}

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_WINDOWS