/*
 * tempsession.h
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#ifndef _tempsession_h
#define _tempsession_h

#include <functional>
#include <unordered_map>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/thread.hpp>

#include "../json/jsoncpp/include/json/json.h"

#include "uuid.h"
#include "session.h"

namespace Fossilizid{
namespace reduce_rpc{

class obj;

class tempsession : public session{
public:
	tempsession(remote_queue::CHANNEL _ch, std::string service_class);
	~tempsession();

public:
	virtual void do_time(boost::uint64_t time);

	virtual void do_pop(boost::shared_ptr<session> session, Json::Value & value);

	virtual bool do_push(boost::shared_ptr<session> session, Json::Value & value);

	virtual void do_logic();

private:
	void do_connect_server(boost::shared_ptr<session> session, Json::Value & value);

private:
	remote_queue::CHANNEL ch; 
	std::string service_class;

};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_session_h