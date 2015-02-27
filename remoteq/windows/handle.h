/*
 * handle.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _handle_h
#define _handle_h

namespace Fossilizid{
namespace remoteq {

enum handle_type{
	handle_channel_type,
	handle_queue_type,
	handle_acceptor_type,
	handle_endpoint_type,
};

struct handle{
	handle_type _handle_type;
};

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_handle_h

#endif //_WINDOWS