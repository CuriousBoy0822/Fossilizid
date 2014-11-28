/*
 * remote_obj.h
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#ifndef _remote_obj_h
#define _remote_obj_h

#include <functional>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include "../json/jsoncpp/include/json/json.h"

#include "obj.h"

namespace Fossilizid{
namespace reduce_rpc{

class session;

class remote_obj : public obj{
public:
	remote_obj(boost::shared_ptr<session> session, Json::Value & value);
	~remote_obj();

public:
	void call_rpc_mothed(Json::Value & value, boost::function<void(Json::Value & ) > rpccallback);

private:
	virtual void call_rpc_mothed(Json::Value & value);

private:
	boost::shared_ptr<session> reqsession; 
	
};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_remote_obj_h