/*
 * module.h
 *
 *  Created on: 2015-1-11
 *      Author: qianqians
 */
#ifndef _module_h
#define _module_h

#include <string>
#include <boost/shared_ptr.hpp>

#include "../interface/channel.h"

#include "../../uuid/uuid.h"

namespace Fossilizid{
namespace juggle{

class module{
public:
	module(std::string modulename, uuid::uuid & moduleid);
	~module();

public:
	/*
	 * get class name
	 */
	std::string module_name();

	/*
	 * get object id
	 */
	uuid::uuid module_id();

	/*
	 * call rpc mothed
	 */
	void call_module_method(boost::shared_ptr<channel> ch, boost::shared_ptr<object> value);

protected:
	std::string _module_name;
	uuid::uuid _module_id;
	
};

} /* namespace juggle */
} /* namespace Fossilizid */

#endif //_obj_h