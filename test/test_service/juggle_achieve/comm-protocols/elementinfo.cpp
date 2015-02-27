/*
 * networkobj.cpp
 *
 *  Created on: 2014-8-20
 *      Author: qianqians
 */
#include "elementinfo.h"
#include "networkcmd.h"
#include "../pool/mempool.h"
#include "../pool/objpool.h"

#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace Fossilizid{
namespace comm_protocols {

elementinfo::elementinfo(){
	type = _None;
}

elementinfo::~elementinfo(){
	switch (type)
	{
	case Fossilizid::comm_protocols::_None:
	case Fossilizid::comm_protocols::_BOOLEAN:
	case Fossilizid::comm_protocols::_I16:
	case Fossilizid::comm_protocols::_I32:
	case Fossilizid::comm_protocols::_I64:
	case Fossilizid::comm_protocols::_U16:
	case Fossilizid::comm_protocols::_U32:
	case Fossilizid::comm_protocols::_U64:
	case Fossilizid::comm_protocols::_F32:
	case Fossilizid::comm_protocols::_F64:
		break;
	case Fossilizid::comm_protocols::_String:
		pool::mempool::deallocator(data.str.str, data.str.len);
		break;
	case Fossilizid::comm_protocols::_OBJ:
		pool::objpool<obj>::deallocator(data._obj, 1);
		break;
	case Fossilizid::comm_protocols::_VECTOR:
		pool::objpool<vector>::deallocator(data._vector, 1);
		break;
	default:
		break;
	}
}

elementinfo::elementinfo(const elementinfo & e){
	*this = e;
}

elementinfo::elementinfo(const boolean & v){
	*this = v;
}

elementinfo::elementinfo(const I16 & v){
	*this = v;
}

elementinfo::elementinfo(const I32 & v){
	*this = v;
}

elementinfo::elementinfo(const I64 & v){
	*this = v;
}

elementinfo::elementinfo(const U16 & v){
	*this = v;
}

elementinfo::elementinfo(const U32 & v){
	*this = v;
}

elementinfo::elementinfo(const U64 & v){
	*this = v;
}

elementinfo::elementinfo(const F32 & v){
	*this = v;
}

elementinfo::elementinfo(const F64 & v){
	*this = v;
}

elementinfo::elementinfo(const String & v){
	*this = v;
}

elementinfo::elementinfo(const obj & v){
	*this = v;
}

elementinfo::elementinfo(const vector & v){
	*this = v;
}

elementinfo & elementinfo::operator = (const elementinfo & e){
	this->~elementinfo();

	type = e.type;
	switch (type)
	{
	case Fossilizid::comm_protocols::_None:
		break;
	case Fossilizid::comm_protocols::_BOOLEAN:
		data.b = e.data.b;
		break;
	case Fossilizid::comm_protocols::_I16:
		data.i16 = e.data.i16;
		break;
	case Fossilizid::comm_protocols::_I32:
		data.i32 = e.data.i32;
		break;
	case Fossilizid::comm_protocols::_I64:
		data.i64 = e.data.i64;
		break;
	case Fossilizid::comm_protocols::_U16:
		data.u16 = e.data.u16;
		break;
	case Fossilizid::comm_protocols::_U32:
		data.u32 = e.data.u32;
		break;
	case Fossilizid::comm_protocols::_U64:
		data.u64 = e.data.u64;
		break;
	case Fossilizid::comm_protocols::_F32:
		data.f32 = e.data.f32;
		break;
	case Fossilizid::comm_protocols::_F64:
		data.f64 = e.data.f64;
		break;
	case Fossilizid::comm_protocols::_String:
		data.str.len = e.data.str.len;
		data.str.str = (char*)pool::mempool::allocator(data.str.len);
		memcpy(data.str.str, e.data.str.str, e.data.str.len);
		break;
	case Fossilizid::comm_protocols::_OBJ:
		data._obj = pool::objpool<obj>::allocator(1);
		::new(data._obj) obj(*e.data._obj);
		break;
	case Fossilizid::comm_protocols::_VECTOR:
		data._vector = pool::objpool<vector>::allocator(1);
		::new(data._vector) vector(*e.data._vector);
		break;
	default:
		break;
	}

	return *this;
}

elementinfo & elementinfo::operator = (const boolean & b){
	this->~elementinfo();

	type = _BOOLEAN;
	data.b = b;

	return *this;
}

elementinfo & elementinfo::operator = (const I16 & i16){
	this->~elementinfo(); 
	
	type = _I16;
	data.i16 = i16;

	return *this;
}

elementinfo & elementinfo::operator = (const I32 & i32){
	this->~elementinfo();

	type = _I32;
	data.i32 = i32;

	return *this;
}

elementinfo & elementinfo::operator = (const I64 & i64){
	this->~elementinfo();

	type = _I64;
	data.i64 = i64;

	return *this;
}

elementinfo & elementinfo::operator = (const U16 & u16){
	this->~elementinfo();

	type = _U16;
	data.u16 = u16;

	return *this;
}

elementinfo & elementinfo::operator = (const U32 & u32){
	this->~elementinfo();

	type = _U32;
	data.u32 = u32;

	return *this;
}

elementinfo & elementinfo::operator = (const U64 & u64){
	this->~elementinfo();

	type = _U64;
	data.u64 = u64;

	return *this;
}

elementinfo & elementinfo::operator = (const F32 & f32){
	this->~elementinfo();

	type = _F32;
	data.f32 = f32;

	return *this;
}

elementinfo & elementinfo::operator = (const F64 & f64){
	this->~elementinfo();

	type = _F64;
	data.f64 = f64;

	return *this;
}

elementinfo & elementinfo::operator = (const String & b){
	this->~elementinfo();

	data.str.len = b.size();
	data.str.str = (char*)pool::mempool::allocator(data.str.len);
	memcpy(data.str.str, b.c_str(), data.str.len);

	return *this;
}

elementinfo & elementinfo::operator = (const obj & b){
	this->~elementinfo();
	data._obj = pool::objpool<obj>::allocator(1);
	new(data._obj) obj(b);
	return *this;
}

elementinfo & elementinfo::operator = (const vector & b){
	this->~elementinfo();
	data._vector = pool::objpool<vector>::allocator(1);
	new(data._vector) vector(b);
	return *this;
}

} /* namespace comm_protocols */
} /* namespace Fossilizid */