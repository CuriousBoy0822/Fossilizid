/*
 * endpoint.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _endpoint_h
#define _endpoint_h

namespace Fossilizid{
namespace remote_queue {

typedef void* ENDPOINT;

ENDPOINT endpoint(char * ip, short port);

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_endpoint_h