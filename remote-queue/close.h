/*
 * queue.h
 *
 *  Created on: 2014-10-3
 *      Author: qianqians
 */
#ifndef _close_h
#define _close_h

namespace Fossilizid{
namespace remote_queue {

typedef void * HANDLE;
void close(HANDLE _handle);

} /* namespace remote_queue */
} /* namespace Fossilizid */

#endif //_close_h