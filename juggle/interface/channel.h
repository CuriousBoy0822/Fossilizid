/*
 * channel.h
 *
 *  Created on: 2015-1-11
 *      Author: qianqians
 */
#ifndef _interface_channel_h
#define _interface_channel_h

#include <boost/shared_ptr.hpp>

#include "object.h"
#include "../service/channel.h"

namespace Fossilizid{
namespace juggle{

class channel : public base::channel{
public:
	/*
	 * push a object to channel
	 */
	virtual void push(boost::shared_ptr<object> v) = 0;
	
	/*
	 * get a object from channel
	 */
	virtual boost::shared_ptr<object> pop() = 0;

};

} /* namespace juggle */
} /* namespace Fossilizid */

#endif //_interface_channel_h