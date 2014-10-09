/*
 * acceptorimpl.cpp
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <Windows.h>

#include <exception>

#include "../../pool/objpool.h"

#include "../acceptor.h"
#include "overlapped.h"
#include "acceptorimpl.h"
#include "channelimpl.h"
#include "queueimpl.h"
#include "endpointimpl.h"

namespace Fossilizid{
namespace remote_queue {

acceptorimlp::acceptorimlp(QUEUE que, ENDPOINT ep){
	_handle_type = handle_acceptor_type;

	sl = socket(AF_INET, SOCK_STREAM, 0);
	sa = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(sl, (sockaddr*)(&((endpointimpl*)ep)->addr), sizeof(SOCKADDR_IN)) != 0){
		throw std::exception("bind error");
	}

	if (listen(sl, 10) != 0){
		throw std::exception("listen error");
	}

	int corenum = 8;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	corenum = info.dwNumberOfProcessors;

	CreateIoCompletionPort((HANDLE)sa, ((queueimpl*)que)->iocp, 0, corenum);

	overlappedex * ovlp = pool::objpool<overlappedex>::allocator(1);
	ovlp->h = (handle*)this;
	ovlp->type = iocp_type_accept;
	AcceptEx(sl, sa, 0, 0, sizeof(SOCKADDR_IN), sizeof(SOCKADDR_IN), 0, (OVERLAPPED*)ovlp);
}

ACCEPTOR acceptor(QUEUE que, ENDPOINT ep){
	acceptorimlp * impl = (acceptorimlp*)pool::objpool<acceptorimlp>::allocator(1);
	new (impl)acceptorimlp(que, ep);

	return (ACCEPTOR)((handle*)impl);
}

CHANNEL accept(ACCEPTOR ap){
	channelimpl * ch = pool::objpool<channelimpl>::allocator(1);
	ch->_handle_type = handle_channel_type;
	ch->s = ((acceptorimlp*)ap)->sa;

	setsockopt(ch->s, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&((acceptorimlp*)ap)->sl, sizeof(((acceptorimlp*)ap)->sl));

	return (CHANNEL)ch;
}

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_WINDOWS