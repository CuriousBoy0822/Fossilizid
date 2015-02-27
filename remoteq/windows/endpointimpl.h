/*
 * endpointimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _endpointimpl_h
#define _endpointimpl_h

#include "handle.h"

namespace Fossilizid{
namespace remoteq{

struct endpointimpl : public handle{
	endpointimpl(){
		_handle_type = handle_endpoint_type;
	}

	SOCKADDR_IN addr;
};

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_endpointimpl_h

#endif //_WINDOWS