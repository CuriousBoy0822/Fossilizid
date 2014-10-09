/*
 * acceptor.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _acceptor_h
#define _acceptor_h

#include "typedef.h"

namespace Fossilizid{
namespace remote_queue{

ACCEPTOR acceptor(QUEUE que, ENDPOINT ep);

CHANNEL accept(ACCEPTOR ap);

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_acceptor_h