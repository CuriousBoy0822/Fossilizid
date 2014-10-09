/*
 * channel.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _channel_h
#define _channel_h

#include "typedef.h"

namespace Fossilizid{
namespace remote_queue {

CHANNEL connect(ENDPOINT ep);

template<class CMD, class CMDTOBUF>
bool push(CHANNEL ch, CMD & cmd, CMDTOBUF fn = CMDTOBUF());

template<class CMD, class BUFTOCMD>
bool pop(CHANNEL ch, CMD & cmd, BUFTOCMD fn = BUFTOCMD());

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_channel_h