/*
 * session.h
 *
 *  Created on: 2014-10-21
 *      Author: qianqians
 */
#ifndef _session_h
#define _session_h

#include "../../third_party/json/json_protocol.h"

namespace Fossilizid{
namespace reduce_rpc{

class session{
public:
	virtual void do_time(boost::uint64_t time) = 0;

	virtual void do_pop(boost::shared_ptr<session> session, Json::Value & value) = 0;

	virtual bool do_push(boost::shared_ptr<session> session, Json::Value & value) = 0;

	virtual void do_logic() = 0;

};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_session_h