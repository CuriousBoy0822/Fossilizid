/*
 * service.h
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#ifndef _service_h
#define _service_h

#include <time.h>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include "session.h"
#include "../remote-queue/queue.h"

#ifdef _WINDOWS
#include <Windows.h>

boost::uint64_t _clock(){
	return GetTickCount64();
}

#endif

namespace Fossilizid{
namespace reduce_rpc{

class service{
public:
	service();
	~service();

	void run();

private:
	boost::uint64_t timestamp;

	remote_queue::QUEUE que;
	boost::unordered_map<remote_queue::CHANNEL, boost::shared_ptr<session> > map_session;

};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_service_h