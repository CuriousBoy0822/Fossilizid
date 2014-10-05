/*
 * overlapped.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#ifndef _overlapped_h
#define _overlapped_h

#include "../channel.h"
#include "handle.h"

namespace Fossilizid{
namespace remote_queue {

enum iocp_type{
	iocp_type_accept,
	iocp_type_send,
	iocp_type_recv,
};

struct overlappedex : public OVERLAPPED{
	handle * h;
	iocp_type type;
};

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_overlapped_h

#endif //_WINDOWS