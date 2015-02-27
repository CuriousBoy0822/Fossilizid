/*
 * channelserver.h
 *
 *  Created on: 2015-1-17
 *      Author: qianqians
 */
#ifndef _json_channelserver_h
#define _json_channelserver_h

#include "../../remoteq/remote_queue.h"

#include <boost/unordered_map.hpp>
#include "../../juggle/interface/juggle.h"

namespace Fossilizid{
namespace reduce{

namespace acceptor{

class channelserver{
public:
	channelserver(boost::shared_ptr<juggle::service> service);
	~channelserver();

	void init(char * ip, short port);

	void poll();

private:
	remoteq::QUEUE que;
	remoteq::ACCEPTOR acp;

	boost::shared_ptr<juggle::service> _service;

	boost::unordered_map<remoteq::CHANNEL, boost::shared_ptr<juggle::channel> > mapchannel;

};

} /* namespace acceptor */

namespace connect{

class channelserver{
public:
	channelserver(boost::shared_ptr<juggle::service> service);
	~channelserver();

	boost::shared_ptr<juggle::channel> connect(char * ip, short port);

	void init();

	void poll();

private:
	void net_work();

private:
	remoteq::QUEUE que;

	boost::shared_ptr<juggle::service> _service;

	boost::unordered_map<remoteq::CHANNEL, boost::shared_ptr<juggle::channel> > mapchannel;

	boost::atomic_bool isrun;

	boost::thread_group thgroup;

};

} /* namespace connect */

} /* namespace reduce */
} /* namespace Fossilizid */

#endif //_json_channelserver_h