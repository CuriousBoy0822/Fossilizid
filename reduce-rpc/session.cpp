/*
 * session.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "session.h"

namespace Fossilizid{
namespace reduce_rpc {

session::session(remote_queue::CHANNEL _ch){
	ch = _ch;
}

session::~session(){
	mapremote_obj.clear();
}

void session::add_time(boost::uint64_t time, std::function<void(boost::uint64_t) > fn){
	boost::mutex::scoped_lock lock(mu);
	maptime.insert(std::make_pair(time, fn));
}

void session::do_time(boost::uint64_t time){
	boost::mutex::scoped_lock lock(mu);
	std::map<boost::uint64_t, std::function<void(boost::uint64_t) > >::iterator end = maptime.upper_bound(time);
	std::map<boost::uint64_t, std::function<void(boost::uint64_t) > >::iterator it = maptime.begin();
	for (; it != end; it++){
		it->second(time);
	}
	maptime.erase(it, end);
}

void session::do_recv(Json::Value & value){
	;
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */