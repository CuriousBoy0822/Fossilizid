/*
 * acceptorimpl.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <Mswsock.h>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

#ifndef _acceptorimlp_h
#define _acceptorimlp_h

#include "handle.h"
#include "../typedef.h"

namespace Fossilizid{
namespace remote_queue{

struct acceptorimlp : public handle{
	acceptorimlp(QUEUE que, ENDPOINT ep);

	char * outbuf;
	SOCKET sl, sa;
	QUEUE que;
};

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_acceptorimlp_h

#endif //_WINDOWS