/*
 * IInterface.h
 *
 *  Created on: 2014-10-30
 *      Author: qianqians
 */
#ifndef _IInterface_h
#define _IInterface_h

#include <boost/bind.hpp>

namespace Fossilizid{
namespace reduce_rpc{

#define RegisterRpcMothed(mothedname, func_str) register_rpc_mothed(mothedname, boost::bind(func_str, this, _1));

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_IInterface_h