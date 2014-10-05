/*
 * networkobj.h
 *
 *  Created on: 2014-8-18
 *      Author: qianqians
 */
#ifndef _networkobj_h
#define _networkobj_h

#include <map>

#include "enumtype.h"

namespace Fossilizid{
namespace comm_protocols {

class cmd{
public:
	cmd();
	~cmd();

	void set(boolean value, String key);
	void set(I16 value, String key);
	void set(I32 value, String key);
	void set(I64 value, String key);
	void set(U16 value, String key);
	void set(U32 value, String key);
	void set(U64 value, String key);
	void set(F32 value, String key);
	void set(F64 value, String key);
	void set(String value, String key);

private:
	struct elementinfo{
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
			OBJ obj;
		} data;
	};
	std::map<String, elementinfo> mapelement;

private:
	char * buff;
	int bufflen, buffindex;
	bool ischange;

	void resize(int bufflen);

	friend char * makebuf(int & len, cmd & cmd);

};

} /* namespace comm_protocols */
} /* namespace Fossilizid */

#endif //_networkobj_h