/*
 * close.cpp
 *
 *  Created on: 2014-10-9
 *      Author: qianqians
 */
#include "../../pool/objpool.h"

#include "../close.h"
#include "queueimpl.h"
#include "endpointimpl.h"
#include "channelimpl.h"
#include "acceptorimpl.h"

namespace Fossilizid{
namespace remoteq {

void close(HANDLE _handle){
	handle * h = (handle*)_handle;
	switch(h->_handle_type)
	{
	case handle_channel_type:
		{
			channelimpl * impl = (channelimpl*)h;
			pool::objpool<channelimpl>::deallocator(impl, 1);
		}
		break;
			
	case handle_queue_type:
		{
			queueimpl * impl = (queueimpl*)h;
			pool::objpool<queueimpl>::deallocator(impl, 1);
		}
		break;

	case handle_acceptor_type:
		{
			acceptorimlp * impl = (acceptorimlp*)h;
			pool::objpool<acceptorimlp>::deallocator(impl, 1);
		}
		break;

	case handle_endpoint_type:
		{
			endpointimpl * impl = (endpointimpl*)h;
			pool::objpool<endpointimpl>::deallocator(impl, 1);
		}
		break;

	default:
		break;
	}
}

} /* namespace remoteq */
} /* namespace Fossilizid */