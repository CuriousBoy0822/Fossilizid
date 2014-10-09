/*
 * service.cpp
 *
 *  Created on: 2014-10-8
 *      Author: qianqians
 */
#include "service.h"

namespace Fossilizid{
namespace reduce_rpc {

service::service(){
	timestamp = time(0) + _clock();
	que = remote_queue::queue();
}

service::~service(){

}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */