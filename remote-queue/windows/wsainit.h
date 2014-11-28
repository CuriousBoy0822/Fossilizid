/*
 * wsainit.h
 *
 *  Created on: 2014-10-21
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#ifndef _wsainit_h
#define _wsainit_h

namespace Fossilizid{
namespace remote_queue {

struct init{
	init(){
		WSADATA _WSADATA;
		WSAStartup(MAKEWORD(2, 2), &_WSADATA);
	}

	~init(){
		WSACleanup();
	}

	static init _init;
};


} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_wsainit_h

#endif //_WINDOWS