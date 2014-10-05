/*
 * enumtype.h
 *
 *  Created on: 2014-8-18
 *      Author: qianqians
 */
#ifndef _enumtype_h
#define _enumtype_h

#include <string>
#include <boost/cstdint.hpp>

namespace Fossilizid{
namespace comm_protocols {

enum network_type{
	_BOOLEAN,
    _I16,
    _I32,
    _I64,
    _U16,
    _U32,
    _U64,
    _F32,
    _F64,
    _String,
    _OBJ,
};

class obj;

typedef bool boolean;
typedef boost::int16_t I16;
typedef boost::int32_t I32;
typedef boost::int64_t I64;
typedef boost::uint16_t U16;
typedef boost::uint32_t U32;
typedef boost::uint64_t U64;
typedef float F32;
typedef double F64;
typedef std::string String;
typedef U64 OBJ;

} /* namespace comm_protocols */
} /* namespace Fossilizid */

#endif //_enumtype_h