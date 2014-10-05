/*
 * channelimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <Windows.h>

#ifndef _channelimpl_h
#define _channelimpl_h

#include "../../pool/mempool.h"
#include "../queue.h"
#include "handle.h"

namespace Fossilizid{
namespace remote_queue{

struct channelimpl : public handle{
	channelimpl(QUEUE que, SOCKET s_){
		_handle_type = handle_channel_type;
		s = s_;

		index = 0;
		buflen = 65536;
		buf = (char*)pool::mempool::allocator(buflen);
	}

	QUEUE que;

	char * buf;
	int buflen, index;
	SOCKET s;
};

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_channelimpl_h

#endif //_WINDOWS