/*
 * ITimeCallback.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "ITimeCallback.h"
#include "../service/service.h"
#include "../service/obj.h"

namespace Fossilizid{
namespace reduce_rpc {

void AddTimeCallBack(boost::uint64_t time, timecallback timefn){
	_service_handle->get_current_obj()->call_add_time(time, timefn);
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */