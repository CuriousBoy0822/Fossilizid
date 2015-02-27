/*
 * channel.h
 *
 *  Created on: 2015-1-17
 *      Author: qianqians
 */
#ifndef _json_channel_h
#define _json_channel_h

#include "../../remoteq/remote_queue.h"

#include <boost/shared_ptr.hpp>

#include "../../container/msque.h"

#include "../../juggle/interface/channel.h"

#include "object.h"

namespace Fossilizid{
namespace jsonplugin{

class channel : public juggle::channel{
public:
	channel(remoteq::CHANNEL _ch){
		ch = _ch;
	}

	~channel(){
	}

	/*
	 * push a object to channel
	 */
	virtual void push(boost::shared_ptr<juggle::object> v){
		remoteq::push(ch, *v, jsonplugin::object_to_buf);
	}
	
	/*
	 * get a object from channel
	 */
	virtual boost::shared_ptr<juggle::object> pop(){
		boost::shared_ptr<juggle::object> v = 0;
		if (que.pop(v)){
			return v;
		}
		return 0;
	}

public:
	void pushcmd(boost::shared_ptr<juggle::object> v){
		que.push(v);
	}

private:
	remoteq::CHANNEL ch;
	container::msque<boost::shared_ptr<juggle::object> > que;

};

} /* namespace json */
} /* namespace Fossilizid */

#endif //_channel_h