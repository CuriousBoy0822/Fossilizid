/*
 * queueimpl.cpp
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <WinSock2.h>

#include "../../pool/objpool.h"

#include "../acceptor.h"
#include "../queue.h"

#include "queueimpl.h"
#include "overlapped.h"
#include "acceptorimpl.h"

namespace Fossilizid{
namespace remoteq {

QUEUE queue(){
	queueimpl * impl = new queueimpl;

	int corenum = 8;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	corenum = info.dwNumberOfProcessors;

	impl->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, corenum);

	return (QUEUE)((handle*)impl);
}

EVENT queue(QUEUE que){
	queueimpl * impl = (queueimpl *)((handle*)que);

	EVENT ev; 
	ev.type = event_type_none;
	ev.handle.acp = 0;
	
	DWORD bytes = 0;
	ULONG_PTR ptr = 0;
	LPOVERLAPPED ovp = 0;
	if (GetQueuedCompletionStatus(impl->iocp, &bytes, &ptr, &ovp, 0)){
		overlappedex * ovlp = static_cast<overlappedex *>(ovp);
		
		if (ovlp->type == iocp_type_accept){
			ev.type = event_type_accept;
			ev.handle.acp = (ACCEPTOR)((acceptorimlp*)ovlp->h);
		} else if (ovlp->type == iocp_type_recv){
			ev.type = event_type_recv;
			ev.handle.acp = (CHANNEL)(ovlp->h);
		} 

		pool::objpool<overlappedex>::deallocator(ovlp, 1);
	}

	return ev;
}

} /* namespace remoteq */
} /* namespace Fossilizid */

#endif //_WINDOWS