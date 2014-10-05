/*
 * acceptorimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <Windows.h>

#ifndef _acceptorimlp_h
#define _acceptorimlp_h

#include "handle.h"
#include "../queue.h"

namespace Fossilizid{
namespace remote_queue{

struct acceptorimlp : public handle{
	acceptorimlp(QUEUE que, ENDPOINT ep);

	SOCKET sl, sa;
	QUEUE que;
};

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_acceptorimlp_h

#endif //_WINDOWS