/*
 * channel.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "../interface/channel.h"
#include "globalhandle.h"

namespace Fossilizid{
namespace juggle {
	
namespace base{

void channel::handle_new_channel(){
	_service_handle->add_rpcsession(static_cast<juggle::channel*>(this));
}

void channel::handle_disconnect_channel(){
	_service_handle->remove_rpcsession(static_cast<juggle::channel*>(this));
}

} /* namespace base */

} /* namespace juggle */
} /* namespace Fossilizid */