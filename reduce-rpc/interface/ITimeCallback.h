/*
 * ITimeCallback.h
 *
 *  Created on: 2014-10-30
 *      Author: qianqians
 */
#ifndef _ITimeCallback_h
#define _ITimeCallback_h

#include <boost/cstdint.hpp>
#include <boost/function.hpp>

namespace Fossilizid{
namespace reduce_rpc{

typedef boost::function<void(boost::uint64_t)> timecallback;
void AddTimeCallBack(boost::uint64_t time, timecallback timefn);

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_ITimeCallback_h