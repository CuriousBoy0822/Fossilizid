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
namespace remoteq{

/*
 * create a ACCEPTOR
 */
ACCEPTOR acceptor(QUEUE que, ENDPOINT ep);

/*
 * accept CHANNEL from ACCEPTOR
 */
CHANNEL accept(ACCEPTOR ap);

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_acceptor_h