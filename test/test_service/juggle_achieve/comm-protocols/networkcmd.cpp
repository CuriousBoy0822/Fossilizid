/*
 * networkobj.cpp
 *
 *  Created on: 2014-8-20
 *      Author: qianqians
 */
#include "networkcmd.h"
#include "../pool/mempool.h"

namespace Fossilizid{
namespace comm_protocols {

cmd::cmd(){
}

cmd::~cmd(){
	mapelement.clear();
}

cmd::cmd(const cmd & _cmd){
	mapelement.insert(_cmd.mapelement.begin(), _cmd.mapelement.end());
}

cmd & cmd::operator = (const cmd & _cmd){
	mapelement.insert(_cmd.mapelement.begin(), _cmd.mapelement.end());
	return *this;
}

void cmd::insert(const boolean & value, String key){
	mapelement[key].type = _BOOLEAN;
	mapelement[key].data.b = value;
}

void cmd::insert(const I16 & value, String key){
	mapelement[key].type = _I16;
	mapelement[key].data.i16 = value;
}

void cmd::insert(const I32 & value, String key){
	mapelement[key].type = _I32;
	mapelement[key].data.i32 = value;
}

void cmd::insert(const I64 & value, String key){
	mapelement[key].type = _I64;
	mapelement[key].data.i64 = value;
}

void cmd::insert(const U16 & value, String key){
	mapelement[key].type = _U16;
	mapelement[key].data.u16 = value;
}

void cmd::insert(const U32 & value, String key){
	mapelement[key].type = _U32;
	mapelement[key].data.u32 = value;
}

void cmd::insert(const U64 & value, String key){
	mapelement[key].type = _U64;
	mapelement[key].data.u64 = value;
}

void cmd::insert(const F32 & value, String key){
	mapelement[key].type = _F32;
	mapelement[key].data.f32 = value;
}

void cmd::insert(const F64 & value, String key){
	mapelement[key].type = _F64;
	mapelement[key].data.f64 = value;
}

void cmd::insert(const String & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	while (0){
		while (0){
			if (find != mapelement.end()){
				if (mapelement[key].type == _String){
					if ((int)value.size() < mapelement[key].data.str.len){
						break;
					}
				} else{
					mapelement[key].type = _String;
				}
			}

			pool::mempool::deallocator(mapelement[key].data.str.str, mapelement[key].data.str.len);
			int tmplen = value.size();
			
			mapelement[key].data.str.len = tmplen;
			mapelement[key].data.str.str = (char*)pool::mempool::allocator(mapelement[key].data.str.len);
		}
	}
	memcpy(mapelement[key].data.str.str, value.c_str(), mapelement[key].data.str.len);
}

void cmd::insert(const vector & v, String key){
	mapelement[key] = v;
}

void cmd::insert(const obj & v, String key){
	mapelement[key] = v;
}

bool cmd::get(boolean & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _BOOLEAN){
		return false;
	}

	value = find->second.data.b;

	return true;
}

bool cmd::get(I16 & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _I16){
		return false;
	}

	value = find->second.data.i16;

	return true;
}

bool cmd::get(I32 & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _I32){
		return false;
	}

	value = find->second.data.i32;

	return true;
}

bool cmd::get(I64 & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _I64){
		return false;
	}

	value = find->second.data.i64;

	return true;
}

bool cmd::get(U16 & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _U16){
		return false;
	}

	value = find->second.data.u16;

	return true;
}

bool cmd::get(U32 & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _U32){
		return false;
	}

	value = find->second.data.u32;

	return true;
}

bool cmd::get(U64 & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _U64){
		return false;
	}

	value = find->second.data.u64;

	return true;
}

bool cmd::get(F32 & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _F32){
		return false;
	}

	value = find->second.data.f32;

	return true;
}

bool cmd::get(F64 & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _F64){
		return false;
	}

	value = find->second.data.f64;

	return true;
}

bool cmd::get(String & value, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _String){
		return false;
	}

	value = find->second.data.str.str;

	return true;
}

bool cmd::get(vector & v, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _String){
		return false;
	}

	v.insert(v.begin(), find->second.data._vector->begin(), find->second.data._vector->end());

	return true;
}

bool cmd::get(obj & v, String key){
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	if (find == mapelement.end()){
		return false;
	}

	if (find->second.type != _String){
		return false;
	}

	v = *find->second.data._obj;

	return true;
}

//std::pair<const char *, const int> makecmdtobuf(const cmd & cmd){
//	;
//}
//
//const cmd makebuftocmd(const char * const buf, const int len);

} /* namespace comm_protocols */
} /* namespace Fossilizid */