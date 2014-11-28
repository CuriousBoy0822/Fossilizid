/*
 * rpcsession.h
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#ifndef _rpcsession_h
#define _rpcsession_h

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

class rpcsession : public session{
public:
	rpcsession(remote_queue::CHANNEL _ch);
	~rpcsession();

	void reset(remote_queue::CHANNEL ch);

public:
	typedef std::unordered_map<uuid, boost::shared_ptr<obj> >::iterator global_obj_iterator;

	void register_global_obj(boost::shared_ptr<obj> obj);

	boost::shared_ptr<obj> get_global_obj(std::string classname);

	void global_obj_lock();

	void global_obj_unlock();

	global_obj_iterator global_obj_begin();

	global_obj_iterator global_obj_end();

public:
	virtual void do_time(boost::uint64_t time);

	virtual void do_pop(boost::shared_ptr<session> session, Json::Value & value);

	virtual bool do_push(boost::shared_ptr<session> session, Json::Value & value);

	virtual void do_logic();

private:
	remote_queue::CHANNEL ch;

private:
	boost::mutex mu_mapremoteobj;
	std::unordered_map<uuid, boost::shared_ptr<obj> > mapremoteobj;
	std::unordered_map<std::string, boost::shared_ptr<obj> > mapremoteobj_classname;

};

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_rpcsession_h