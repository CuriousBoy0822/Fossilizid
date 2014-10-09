/*
 * remote_obj.h
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#ifndef _remote_obj_h
#define _remote_obj_h

#include <functional>
#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include "../remote-queue/queue.h"
#include "../../jsoncpp/jsoncpp/include/json/json.h"

#include "session.h"

namespace Fossilizid{
namespace reduce_rpc{

class remote_obj{
public:
	remote_obj();
	~remote_obj();

	void call_rpc_mothed();

private:
	std::string class_name;
	boost::uint64_t objid;
	boost::shared_ptr<session> _session;
	std::map<std::string, std::function<void(Json::Value &) > > maprpccall;
	
};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_remote_obj_h