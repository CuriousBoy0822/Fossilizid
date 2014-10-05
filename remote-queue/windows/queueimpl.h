/*
 * queueimpl.h
 *
 *  Created on: 2014-10-5
 *      Author: qianqians
 */
#ifdef _WINDOWS
#include <Windows.h>

#ifndef _queueimpl_h
#define _queueimpl_h

#include "handle.h"
#include "../queue.h"

#include "../../container/msque.h"

namespace Fossilizid{
namespace remote_queue {

struct queueimpl : public handle{
	queueimpl(){
		_handle_type = handle_queue_type;
	}

	container::msque<EVENT> evque;
	HANDLE iocp;
};

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_queueimpl_h

#endif //_WINDOWS