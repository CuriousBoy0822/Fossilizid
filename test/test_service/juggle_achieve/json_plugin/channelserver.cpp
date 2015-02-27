/*
 * channelserver.cpp
 *
 *  Created on: 2015-1-17
 *      Author: qianqians
 */
#include "../channelserver.h"

#include "channel.h"

#include <juggle.h>

#include <boost/make_shared.hpp>

namespace Fossilizid{

namespace reduce{

namespace acceptor{

channelserver::channelserver(boost::shared_ptr<juggle::service> service){
	_service = service;
}

channelserver::~channelserver(){
}

void channelserver::init(char * ip, short port){
	que = remoteq::queue();
	acp = remoteq::acceptor(que, remoteq::endpoint(ip, port));

	_service->init();
}

void channelserver::poll(){
	remoteq::EVENT ev = remoteq::queue(que);
	switch (ev.type)
	{
	case remoteq::event_type_none:
		break;
				
	case remoteq::event_type_accept:
		{
			remoteq::CHANNEL ch = remoteq::accept(ev.handle.acp);
			if (ch != 0){
				boost::shared_ptr<jsonplugin::channel> c = boost::make_shared<jsonplugin::channel>(ch);
				mapchannel.insert(std::make_pair(ch, c));
				Fossilizid::juggle::_service_handle->add_rpcsession(c.get());
			}
		}
		break;

	case remoteq::event_type_recv:
		{	
			remoteq::CHANNEL ch = ev.handle.ch;
				
			boost::shared_ptr<juggle::object> v = boost::make_shared<jsonplugin::object>();
			while (remoteq::pop(ch, *v, jsonplugin::buf_to_object)){
				if (!v->hasfield("suuid")){
					continue;
				}

				boost::static_pointer_cast<jsonplugin::channel>(mapchannel[ch])->pushcmd(v);
			}
		}
		break;

	case remoteq::event_type_disconnect:
		{
			Fossilizid::juggle::_service_handle->remove_rpcsession(mapchannel[ev.handle.ch].get());
			mapchannel.erase(ev.handle.ch);
			remoteq::close(ev.handle.ch);
		}
		break;

	default:
		break;
	}

	_service->poll();
}

} /* namespace acceptor */

namespace connect{

channelserver::channelserver(boost::shared_ptr<juggle::service> service){
	isrun.store(true);
	_service = service;
}
	
channelserver::~channelserver(){
	isrun.store(false);
	thgroup.join_all();
}

boost::shared_ptr<juggle::channel> channelserver::connect(char * ip, short port){
	remoteq::CHANNEL ch = remoteq::connect(remoteq::endpoint(ip, port), que);
	if (ch != 0){
		boost::shared_ptr<jsonplugin::channel> c = boost::make_shared<jsonplugin::channel>(ch);
		mapchannel.insert(std::make_pair(ch, c));
		Fossilizid::juggle::_service_handle->add_rpcsession(c.get());
		
		return c;
	}

	return 0;
}

void channelserver::init(){
	que = remoteq::queue();

	_service->init();

	auto loop = [this](){
		while(isrun.load()){
			net_work();
		}
	};
	thgroup.create_thread(loop);
}

void channelserver::net_work(){
	remoteq::EVENT ev = remoteq::queue(que);
	switch (ev.type)
	{
	case remoteq::event_type_none:
		break;

	case remoteq::event_type_recv:
		{	
			remoteq::CHANNEL ch = ev.handle.ch;
				
			boost::shared_ptr<juggle::object> v = boost::make_shared<jsonplugin::object>();
			while (remoteq::pop(ch, *v, jsonplugin::buf_to_object)){
				if (!v->hasfield("suuid")){
					continue;
				}

				boost::static_pointer_cast<jsonplugin::channel>(mapchannel[ch])->pushcmd(v);
			}
		}
		break;

	case remoteq::event_type_disconnect:
		{
			Fossilizid::juggle::_service_handle->remove_rpcsession(mapchannel[ev.handle.ch].get());
			mapchannel.erase(ev.handle.ch);
			remoteq::close(ev.handle.ch);
		}
		break;

	default:
		break;
	}
}

void channelserver::poll(){
	net_work();

	_service->poll();
}

} /* namespace connect */

} /* namespace reduce */

} /* namespace Fossilizid */