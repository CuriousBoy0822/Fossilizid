/*
 * session.h
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#ifndef _session_h
#define _session_h

#include <functional>
#include <map>
#include <string>

#include <boost/cstdint.hpp>

#include "../remote-queue/queue.h"
#include "../../jsoncpp/jsoncpp/include/json/json.h"

#include "remote_obj.h"

namespace Fossilizid{
namespace reduce_rpc{

class session{
public:
	session(remote_queue::CHANNEL _ch);
	~session();

	void add_time(boost::uint64_t time, std::function<void(boost::uint64_t) > fn);

	void do_time(boost::uint64_t time);

	void do_recv(Json::Value & value);

private:
	remote_queue::CHANNEL ch;

	std::map<boost::uint64_t, boost::shared_ptr<remote_obj> > mapremote_obj;

};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_session_h