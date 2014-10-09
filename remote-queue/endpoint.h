/*
 * endpoint.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _endpoint_h
#define _endpoint_h

#include "typedef.h"

namespace Fossilizid{
namespace remote_queue {

ENDPOINT endpoint(char * ip, short port);

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_endpoint_h