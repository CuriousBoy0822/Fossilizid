/*
 * semaphore.cpp
 *
 *  Created on: 2015-1-14
 *      Author: qianqians
 */
#include "../interface/semaphore.h"
#include "../../context/context.h"

#include "globalhandle.h"

namespace Fossilizid{
namespace juggle {

semaphore::semaphore(){
	_signal = 0;
	wait_ct = 0;
}

semaphore::~semaphore(){
}

void semaphore::post(boost::shared_ptr<object> signal){
	boost::mutex::scoped_lock l(mu_signal);
	_signal = signal;
	l.unlock();

	if (wait_ct != 0){
		_service_handle->wake_up_context(wait_ct);
	}
}

boost::shared_ptr<object> semaphore::wait(){
	do{
		boost::mutex::scoped_lock l(mu_signal);
		if (_signal != 0){
			break;
		}
		wait_ct = _service_handle->get_current_context();
		l.unlock();

		_service_handle->scheduler();
	} while (0);

	return _signal;
}

} /* namespace juggle */
} /* namespace Fossilizid */