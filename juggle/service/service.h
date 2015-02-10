/*
 * service.h
 *
 *  Created on: 2015-1-11
 *      Author: qianqians
 */
#ifndef _service_base_h
#define _service_base_h

#include <set>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include "../../uuid/uuid.h"
#include "../../context/context.h"

#include "../interface/service.h"
#include "../interface/channel.h"

#include "module.h"

namespace Fossilizid{
namespace juggle{

class juggleservice : public service{
public:
	juggleservice();
	~juggleservice();

public:
	/*
	 * initialise service
	 */
	virtual void init();

	/*
	 * drive service work
	 */
	virtual void poll();

public:
	void add_rpcsession(channel * ch);

	void remove_rpcsession(channel * ch);
	
	void register_module_method(std::string methodname, boost::function<void(channel *, boost::shared_ptr<object>)> modulemethod);

	void register_rpc_callback(uuid::uuid, boost::function<void(boost::shared_ptr<object>)> callback);
	
	context::context get_current_context();

	void wake_up_context(context::context ct);

	void scheduler();

private:
	void set_current_context(context::context _context);

	void loop_main();

private:
	boost::thread_specific_ptr<context::context> tss_current_context;

private:
	boost::mutex mu_method_map;
	boost::unordered_map<std::string, boost::function<void(channel *, boost::shared_ptr<object>)> > method_map;
	
	boost::mutex mu_method_callback_map;
	boost::unordered_map<uuid::uuid, boost::function<void(boost::shared_ptr<object>)> > method_callback_map;

private:
	boost::mutex mu_wake_up_vector;
	std::vector<context::context> wait_weak_up_context;

private:
	boost::mutex mu_channel;
	std::set<channel * > set_channel;

	boost::mutex mu_new_channel;
	std::vector<channel * > array_new_channel;

};

void create_module();

} /* namespace juggle */
} /* namespace Fossilizid */

#endif //_service_base_h