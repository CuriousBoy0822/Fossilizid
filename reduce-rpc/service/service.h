/*
 * service.h
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#ifndef _service_h
#define _service_h

#include "../../remote-queue/remote_queue.h"

#include <time.h>

#include <unordered_map>
#include <tuple>
#include <stack>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>

#include "../../context/context.h"
#include "../../third_party/json/json_protocol.h"

#include "uuid.h"

namespace Fossilizid{
namespace reduce_rpc{

class rpcsession;
class session;
class obj;

struct semaphore;

class service{
public:
	service();
	~service();

public:
	boost::uint64_t unixtime();

public:	
	void init();

	void join();

public:
	boost::shared_ptr<session> create_rpcsession(uuid epuuid, remote_queue::CHANNEL ch);

	boost::shared_ptr<session> get_rpcsession(uuid epuuid);

public:
	typedef std::unordered_map<uuid, boost::shared_ptr<obj> >::iterator global_obj_iterator;

	void register_global_obj(boost::shared_ptr<obj> obj);

	boost::shared_ptr<obj> get_global_obj(std::string classname);

	void global_obj_lock();

	void global_obj_unlock();

	global_obj_iterator global_obj_begin();

	global_obj_iterator global_obj_end();

public:
	void push_current_session(boost::shared_ptr<session> session);

	void pop_current_session();

	boost::shared_ptr<session> get_current_session();

public:
	void push_current_obj(boost::shared_ptr<obj> session);

	void pop_current_obj();

	boost::shared_ptr<obj> get_current_obj();

public:
	void set_current_context(context::context * _context);

	context::context * get_current_context();

	boost::shared_ptr<Json::Value> wait(uuid _uuid, boost::uint64_t wait_time);

protected:
	virtual void run_network() = 0;

protected:
	boost::atomic_bool isrun;
	boost::thread_group _thread_group;

protected:
	boost::uint64_t clockstamp;
	boost::uint64_t timestamp;

protected:
	boost::mutex mu_map_global_obj;
	std::unordered_map<uuid, boost::shared_ptr<obj> > map_global_obj;
	std::unordered_map<std::string, boost::shared_ptr<obj> > map_global_obj_classname;
	
protected:
	boost::shared_mutex mu_map_session;
	std::unordered_map<remote_queue::CHANNEL, boost::shared_ptr<session> > map_session;

	boost::thread_specific_ptr<std::stack<boost::shared_ptr<session> > > tsp_current_session;

	boost::shared_mutex mu_map_channel;
	std::unordered_map<uuid, remote_queue::CHANNEL> map_channel;

	boost::shared_mutex mu_map_uuid_session;
	std::unordered_map<uuid, boost::shared_ptr<rpcsession> > map_uuid_session;

protected:
	boost::mutex mu_wait_context_list;
	std::unordered_map<uuid, std::tuple<uuid, context::context *, boost::uint64_t, boost::shared_ptr<Json::Value> > > wait_context_list;
	
	boost::mutex mu_wake_up_set;
	std::set<uuid> wake_up_set;

	boost::thread_specific_ptr<context::context> tsp_context;

	boost::thread_specific_ptr<context::context> tsp_loop_main_context;

protected:
	boost::thread_specific_ptr<std::stack<boost::shared_ptr<obj> > > tsp_current_obj;

};

extern service * _service_handle;

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_service_h