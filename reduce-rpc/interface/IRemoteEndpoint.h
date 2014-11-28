/*
 * IRemoteEndpoint.h
 *
 *  Created on: 2014-10-30
 *      Author: qianqians
 */
#ifndef _IRemoteEndpoint_h
#define _IRemoteEndpoint_h

#include "../service/connectservice.h"
	
namespace Fossilizid{	
namespace reduce_rpc{	

class IRemoteEndpoint{
public:	
	IRemoteEndpoint(){
		_connectservice.init();
	}

	void ConnectService(char * ip, short port);

private:
	connectservice _connectservice;

};	

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_IRemoteEndpoint_h