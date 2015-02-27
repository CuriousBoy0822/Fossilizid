/*
 * service.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "service.h"
#include <boost/make_shared.hpp>

#include "../../pool/factory.h"
#include "globalhandle.h"

#ifdef _WINDOWS
#include <Windows.h>
boost::uint64_t _clock(){
	return GetTickCount64();
}
#endif

namespace Fossilizid{
namespace juggle {

boost::shared_ptr<juggleservice> _service_handle = 0;
context::context _main_context = 0;

boost::shared_ptr<service> create_service(){
	_service_handle = boost::make_shared<juggleservice>();
	return boost::static_pointer_cast<service>(_service_handle);
}

juggleservice::juggleservice(){
}

juggleservice::~juggleservice(){
}

void juggleservice::init(){
	clockstamp = _clock();
	timestamp = time(0);

	create_module();

	context::context current_ct = context::makecontext();
	set_current_context(current_ct);

	_main_context = context::getcontext(boost::bind(&juggleservice::loop_main, this));
}

void juggleservice::poll(){
	context::context current_ct = get_current_context();
	boost::mutex::scoped_lock l(mu_wake_up_vector);
	wait_weak_up_context.push_back(current_ct);
	l.unlock();

	context::yield(_main_context);
}

boost::uint64_t juggleservice::unixtime(){
	return timestamp;
}

void juggleservice::loop_main(){
	while(1){
		{
			timestamp += _clock() - clockstamp;

			boost::mutex::scoped_lock lmu_channel(mu_channel);
			boost::mutex::scoped_lock lmu_method_map(mu_method_map);
			boost::mutex::scoped_lock lmu_method_callback_map(mu_method_callback_map);
			for(auto ch : set_channel){
				boost::shared_ptr<object> cmd = ch->pop();
				if (cmd == 0){
					continue;
				}

				if ((*cmd)["rpcevent"].asstring() == "call_rpc_method"){
					auto find = method_map.find((*cmd)["method"].asstring());
					if (find != method_map.end()){
						find->second(ch, cmd);
					}
				}else if ((*cmd)["rpcevent"].asstring() == "reply_rpc_method"){
					auto find = method_callback_map.find((*cmd)["suuid"].asstring());
					if (find != method_callback_map.end()){
						find->second(cmd);
					}
				}
			}
		}

		{
			boost::mutex::scoped_lock lmu_new_channel(mu_new_channel);
			boost::mutex::scoped_lock lmu_channel(mu_channel);
			for(auto ch : array_new_channel){
				set_channel.insert(ch);
			}
		}

		{
			boost::mutex::scoped_lock l(mu_wake_up_vector);
			if (!wait_weak_up_context.empty()){
				context::context ct = wait_weak_up_context.back();
				wait_weak_up_context.pop_back();
				l.unlock();

				context::yield(ct);
			}
		}


		{
			boost::mutex::scoped_lock l(mu_vsemaphore);
			if (!vsemaphore.empty()){
				auto find = vsemaphore.find(timestamp);
				auto ct = find->second;
				vsemaphore.erase(find);
				l.unlock();

				context::yield(ct);
			}
		}
	}
}

void juggleservice::add_rpcsession(channel * ch){
	boost::mutex::scoped_lock l(mu_new_channel);
	array_new_channel.push_back(ch);
}

void juggleservice::remove_rpcsession(channel * ch){
	boost::mutex::scoped_lock l(mu_channel);
	set_channel.erase(ch);
}

void juggleservice::register_semaphore(semaphore * _semaphore){
	boost::mutex::scoped_lock l(mu_vsemaphore);
	vsemaphore.insert(std::make_pair(_semaphore->_timeout, _semaphore->wait_ct));
}

void juggleservice::register_module_method(std::string methodname, boost::function<void(channel *, boost::shared_ptr<object>)> modulemethod){
	boost::mutex::scoped_lock l(mu_method_map);
	method_map.insert(std::make_pair(methodname, modulemethod));
}

void juggleservice::register_rpc_callback(uuid::uuid suuid, boost::function<void(boost::shared_ptr<object>)> callback){
	boost::mutex::scoped_lock l(mu_method_callback_map);
	method_callback_map.insert(std::make_pair(suuid, callback));
}

context::context juggleservice::get_current_context(){
	context::context * pcontext = tss_current_context.get();
	if (pcontext == 0){
		return 0;
	}
	return *pcontext;
}

void juggleservice::wake_up_context(context::context ct){
	boost::mutex::scoped_lock l(mu_wake_up_vector);
	wait_weak_up_context.push_back(ct);
}

void juggleservice::set_current_context(context::context _context){
	context::context * pcontext = tss_current_context.get();
	if (pcontext == 0){
		pcontext = pool::factory::create<context::context>();
		tss_current_context.reset(pcontext);
	}
	*pcontext = _context;
}

void juggleservice::scheduler(){
	context::context _context = get_current_context();

	if (_main_context != 0){
		if (_main_context == _context){
			context::context ct = context::getcontext(boost::bind(&juggleservice::loop_main, this));
			set_current_context(ct);
			
			context::yield(ct);
		} else {
			context::yield(_main_context);
		}
	}else {
		throw std::exception("_tsp_loop_main_context is null");
	}
}


} /* namespace juggle */
} /* namespace Fossilizid */