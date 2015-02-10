/*
 * channel.h
 *
 *  Created on: 2015-1-11
 *      Author: qianqians
 */
#ifndef _channel_base_h
#define _channel_base_h

namespace Fossilizid{
namespace juggle{

namespace base{

class channel{
public:
	/*
	 * create a new channel
	 */
	void handle_new_channel();

	/*
	 * release the channel
	 */
	void handle_disconnect_channel();
	
};

} /* namespace base */

} /* namespace juggle */
} /* namespace Fossilizid */

#endif //_channel_base_h