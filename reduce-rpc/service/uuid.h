/*
 * uuid.h
 *
 *  Created on: 2014-10-14
 *      Author: qianqians
 */
#ifndef _uuid_h
#define _uuid_h

#include <string>
#include <time.h>

#ifdef _WINDOWS

#include <windows.h> 
#include <iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")

#endif

namespace Fossilizid{
namespace reduce_rpc{

typedef std::string uuid;

uuid UUID();

} /* namespace reduce_rpc */
} /* namespace Fossilizid */

#endif //_uuid_h