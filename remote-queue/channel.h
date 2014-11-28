/*
 * channel.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _channel_h
#define _channel_h

#include "typedef.h"

#ifdef _WINDOWS
#include "windows/channelimpl.h"
#endif

namespace Fossilizid{
namespace remote_queue {

CHANNEL connect(ENDPOINT ep, QUEUE que = 0);

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_channel_h