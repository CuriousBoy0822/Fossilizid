/*
 * acceptor.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _acceptor_h
#define _acceptor_h

#include "endpoint.h"
#include "channel.h"

namespace Fossilizid{
namespace remote_queue{

typedef void* ACCEPTOR;

ACCEPTOR acceptor(QUEUE que, ENDPOINT ep);

CHANNEL accept(ACCEPTOR ap);

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_acceptor_h