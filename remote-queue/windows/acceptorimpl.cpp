/*
 * acceptorimpl.cpp
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS

#include <exception>

#include "../../pool/objpool.h"

#include "overlapped.h"

#include "../acceptor.h"
#include "acceptorimpl.h"

#include "channelimpl.h"
#include "queueimpl.h"
#include "endpointimpl.h"

namespace Fossilizid{
namespace remote_queue {

acceptorimlp::acceptorimlp(QUEUE _que, ENDPOINT ep){
	_handle_type = handle_acceptor_type;
	que = _que;

	sl = socket(AF_INET, SOCK_STREAM, 0);
	sa = socket(AF_INET, SOCK_STREAM, 0);

	outbuf = new char[1024];

	if (bind(sl, (sockaddr*)(&((endpointimpl*)((handle*)ep))->addr), sizeof(SOCKADDR_IN)) != 0){
		throw std::exception("bind error");
	}

	if (listen(sl, 10) != 0){
		throw std::exception("listen error");
	}

	int corenum = 8;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	corenum = info.dwNumberOfProcessors;

	if (CreateIoCompletionPort((HANDLE)sl, ((queueimpl*)((handle*)_que))->iocp, 0, corenum) != ((queueimpl*)((handle*)_que))->iocp){
		throw std::exception("CreateIoCompletionPort error", WSAGetLastError());
	}

	memset(outbuf, 0, sizeof(outbuf));

	overlappedex * ovlp = pool::objpool<overlappedex>::allocator(1);
	new (ovlp) overlappedex();
	ovlp->h = (handle*)this;
	ovlp->type = iocp_type_accept;
	OVERLAPPED * ovp = static_cast<OVERLAPPED *>(ovlp);
	memset(ovp, 0, sizeof(OVERLAPPED));
	if (!AcceptEx(sl, sa, outbuf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, ovp)){
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING){
			throw std::exception("acceptorimlp", error);
		}
	}
}

ACCEPTOR acceptor(QUEUE que, ENDPOINT ep){
	acceptorimlp * impl = (acceptorimlp*)pool::objpool<acceptorimlp>::allocator(1);
	new (impl)acceptorimlp(que, ep);

	return (ACCEPTOR)((handle*)impl);
}

CHANNEL accept(ACCEPTOR ap){
	channelimpl * ch = pool::objpool<channelimpl>::allocator(1);
	new (ch)channelimpl(((acceptorimlp*)((handle*)ap))->que, ((acceptorimlp*)((handle*)ap))->sa);

	setsockopt(ch->s, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&((acceptorimlp*)((handle*)ap))->sl, sizeof(((acceptorimlp*)((handle*)ap))->sl));

	CreateIoCompletionPort((HANDLE)ch->s, ((queueimpl*)((handle*)ch->que))->iocp, 0, 0);

	{
		WSABUF * wsabuf = pool::objpool<WSABUF>::allocator(1);
		wsabuf->buf = ch->buf;
		wsabuf->len = 0;
		DWORD bytes = 0;
		DWORD flags = 0;
		overlappedex * ovp = pool::objpool<overlappedex>::allocator(1);
		new (ovp)overlappedex();
		ovp->h = (handle*)ch;
		ovp->type = iocp_type_recv;
		OVERLAPPED * ovp_ = static_cast<OVERLAPPED *>(ovp);
		memset(ovp_, 0, sizeof(OVERLAPPED));
		WSARecv(ch->s, wsabuf, 1, &bytes, &flags, ovp_, 0);
	}

	{
		((acceptorimlp*)((handle*)ap))->sa = socket(AF_INET, SOCK_STREAM, 0);

		memset(((acceptorimlp*)((handle*)ap))->outbuf, 0, sizeof(((acceptorimlp*)((handle*)ap))->outbuf));
		overlappedex * ovlp = pool::objpool<overlappedex>::allocator(1);
		new (ovlp)overlappedex();
		ovlp->h = (handle*)ap;
		ovlp->type = iocp_type_accept;
		OVERLAPPED * ovp_ = static_cast<OVERLAPPED *>(ovlp);
		memset(ovp_, 0, sizeof(OVERLAPPED));
		AcceptEx(((acceptorimlp*)((handle*)ap))->sl, ((acceptorimlp*)((handle*)ap))->sa, ((acceptorimlp*)((handle*)ap))->outbuf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, 0, ovp_);
	}

	return (CHANNEL)((handle*)ch);
}

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_WINDOWS