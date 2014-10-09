/*
 * networkobj.cpp
 *
 *  Created on: 2014-8-20
 *      Author: qianqians
 */
#include "../../pool/objpool.h"

#include "../channel.h"
#include "queueimpl.h"
#include "overlapped.h"
#include "endpointimpl.h"
#include "channelimpl.h"

namespace Fossilizid{
namespace remote_queue {

CHANNEL connect(QUEUE que, ENDPOINT ep){
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (::connect(s, (SOCKADDR*)&((endpointimpl*)ep)->addr, sizeof(SOCKADDR_IN) != 0)){
		closesocket(s);
		return 0;
	}

	channelimpl * ch = pool::objpool<channelimpl>::allocator(1);
	ch->s = s;
	ch->que = que;
	ch->_handle_type = handle_channel_type;

	return (CHANNEL)((handle*)ch);
}

template<class CMD, class CMDTOBUF>
bool push(CHANNEL ch, CMD & cmd, CMDTOBUF fn = CMDTOBUF()){
	std::pair<char *, int> buf = fn(cmd);

	WSABUF * wsabuf = pool::objpool<WSABUF>::allocator(1);
	wsabuf->buf = buf.first;
	wsabuf->len = buf.second;
	DWORD bytes = 0;
	overlappedex * ovp = pool::objpool<overlappedex>::allocator(1);
	ovp->h = (handle*)(channelimpl*)ch;
	ovp->type = iocp_type_send;
	if (WSASend(((channelimpl*)ch)->s, wsabuf, 1, &bytes, 0, (OVERLAPPED*)ovp, 0) == SOCKET_ERROR){
		if (WSAGetLastError() != WSA_IO_PENDING){
			return false;
		}
	}

	return true;
}

template<class CMD, class BUFTOCMD>
bool pop(CHANNEL ch, CMD & cmd, BUFTOCMD fn = BUFTOCMD()){
	int recvlen = 0;

	while (1){
		int len = recv(((channelimpl*)ch)->s, buf, buflen, 0);
		
		if (len > 0 && len < buflen){
			break;
		}

		if (len < 0){
			if (WSAGetLastError() != WSAEWOULDBLOCK){
				break;
			}
		}

		if (len == 0){
			EVENT ev;
			ev.handle.ch = ch;
			ev.type = event_type_disconnect;
			((queueimpl*)((channelimpl*)ch)->que)->evque.push(ev);

			break;
		}

		recvlen += len;
		int buflen = ((channelimpl*)ch)->buflen;
		((channelimpl*)ch)->buflen *= 2;
		char * tmp = ((channelimpl*)ch)->buf;
		((channelimpl*)ch)->buf = (char*)pool::mempool::allocator(buflen);
		memcpy(((channelimpl*)ch)->buf, tmp, buflen);
		pool::mempool::deallocator(tmp, buflen);
	}

	if (((channelimpl*)ch)->index > 0){
		int cmdbuflen = 0;
		if (!fn(cmd, ((channelimpl*)ch)->buf + ((channelimpl*)ch)->index, cmdbuflen)){
			return false;
		}
		((channelimpl*)ch)->index += cmdbuflen;

		if (((channelimpl*)ch)->index == ((channelimpl*)ch)->buflen){
			((channelimpl*)ch)->index = 0;
		}
	}

	return true;
}

} /* namespace remote_queue */
} /* namespace Fossilizid */