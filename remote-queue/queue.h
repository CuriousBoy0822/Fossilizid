/*
 * queue.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _queue_h
#define _queue_h

#include "channel.h"
#include "acceptor.h"
#include "endpoint.h"
#include "close.h"

namespace Fossilizid{
namespace remote_queue {

QUEUE queue();

enum event_type{
	event_type_none,
	event_type_accept,
	event_type_recv,
	event_type_disconnect,
};

struct EVENT{
	union { ACCEPTOR acp; CHANNEL ch; } handle;
	event_type type;
};

EVENT queue(QUEUE que);

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_queue_h