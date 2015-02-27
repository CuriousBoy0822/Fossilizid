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

class cmd;
class vector;

enum network_type{
	_None,
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
	_VECTOR
};

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
typedef Fossilizid::comm_protocols::cmd obj;
typedef Fossilizid::comm_protocols::vector vector;

} /* namespace comm_protocols */
} /* namespace Fossilizid */

#endif //_enumtype_h