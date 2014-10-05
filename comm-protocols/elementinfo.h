/*
 * elementinfo.h
 *
 *  Created on: 2014-9-13
 *      Author: qianqians
 */
#ifndef _elementinfo_h
#define _elementinfo_h

#include "enumtype.h"

namespace Fossilizid{
namespace comm_protocols {

struct elementinfo{
	elementinfo();
	~elementinfo();

	elementinfo(const elementinfo & e);
	elementinfo(const boolean & v);
	elementinfo(const I16 & v);
	elementinfo(const I32 & v);
	elementinfo(const I64 & v);
	elementinfo(const U16 & v);
	elementinfo(const U32 & v);
	elementinfo(const U64 & v);
	elementinfo(const F32 & v);
	elementinfo(const F64 & v);
	elementinfo(const String & v);
	elementinfo(const obj & v);
	elementinfo(const vector & v);

	elementinfo & operator = (const elementinfo & e);
	elementinfo & operator = (const boolean & b);
	elementinfo & operator = (const I16 & b);
	elementinfo & operator = (const I32 & b);
	elementinfo & operator = (const I64 & b);
	elementinfo & operator = (const U16 & b);
	elementinfo & operator = (const U32 & b);
	elementinfo & operator = (const U64 & b);
	elementinfo & operator = (const F32 & b);
	elementinfo & operator = (const F64 & b);
	elementinfo & operator = (const String & b);
	elementinfo & operator = (const obj & b);
	elementinfo & operator = (const vector & b);

	network_type type;
	union {
		boolean b;
		I16 i16;
		I32 i32;
		I64 i64;
		U16 u16;
		U32 u32;
		U64 u64;
		F32 f32;
		F64 f64;
		struct { int len; char * str; } str;
		obj * _obj;
		vector * _vector;
	} data;

};

} /* namespace comm_protocols */
} /* namespace Fossilizid */

#endif //_mempool_h