/*
 * networkobj.cpp
 *
 *  Created on: 2014-8-20
 *      Author: qianqians
 */
#include "networkcmd.h"
#include "mempool.h"

namespace Fossilizid{
namespace comm_protocols {

static int normalbufflen = 4096;

cmd::cmd() : ischange(false), buff(0), bufflen(0), buffindex(0){
	resize(normalbufflen);
}

cmd::~cmd(){
	mempool::deallocator(buff, bufflen);
}

void cmd::resize(int _bufflen){
	char * tmpbuff = (char*)mempool::allocator(_bufflen);
	memcpy(tmpbuff, buff, bufflen);
	if (buff != 0){
		mempool::deallocator(buff, bufflen);
	}
	buff = tmpbuff;
	bufflen = _bufflen;
}

void cmd::set(boolean value, String key){
	ischange = true;
	mapelement[key].type = _BOOLEAN;
	mapelement[key].data.b = value;
}

void cmd::set(I16 value, String key){
	ischange = true;
	mapelement[key].type = _I16;
	mapelement[key].data.i16 = value;
}

void cmd::set(I32 value, String key){
	ischange = true;
	mapelement[key].type = _I32;
	mapelement[key].data.i32 = value;
}

void cmd::set(I64 value, String key){
	ischange = true;
	mapelement[key].type = _I64;
	mapelement[key].data.i64 = value;
}

void cmd::set(U16 value, String key){
	ischange = true;
	mapelement[key].type = _U16;
	mapelement[key].data.u16 = value;
}

void cmd::set(U32 value, String key){
	ischange = true;
	mapelement[key].type = _U32;
	mapelement[key].data.u32 = value;
}

void cmd::set(U64 value, String key){
	ischange = true;
	mapelement[key].type = _U64;
	mapelement[key].data.u64 = value;
}

void cmd::set(F32 value, String key){
	ischange = true;
	mapelement[key].type = _F32;
	mapelement[key].data.f32 = value;
}

void cmd::set(F64 value, String key){
	ischange = true;
	mapelement[key].type = _F64;
	mapelement[key].data.f64 = value;
}

void cmd::set(String value, String key){
	ischange = true;
	std::map<String, elementinfo>::iterator find = mapelement.find(key);
	while (0){
		while (0){
			if (find != mapelement.end()){
				if (mapelement[key].type == _String){
					if (value.size() < mapelement[key].data.str.len){
						break;
					}
				} else{
					mapelement[key].type = _String;
					break;
				}
			}

			mempool::deallocator(mapelement[key].data.str.str, mapelement[key].data.str.len);
			int tmplen = value.size();
			if (tmplen <= _4K){
				tmplen = _4K;
			} else if (tmplen <= _8K){
				tmplen = _8K;
			} else if (tmplen <= _16K){
				tmplen = _16K;
			} else if (tmplen <= _32K){
				tmplen = _32K;
			} else if (tmplen <= _64K){
				tmplen = _64K;
			} else{
				tmplen = (tmplen + _4K - 1) / _4K * _4K;
			}
			mapelement[key].data.str.len = tmplen;
			mapelement[key].data.str.str = (char*)mempool::allocator(mapelement[key].data.str.len);
		}
	}
	memcpy(mapelement[key].data.str.str, value.c_str(), mapelement[key].data.str.len);
}

char * makebuf(int & len, cmd & cmd){
	cmd.buffindex = sizeof(I32);
	for (std::map<String, cmd::elementinfo>::iterator iter = cmd.mapelement.begin(); iter != cmd.mapelement.end(); iter++){
		switch (iter->second.type){
			case _BOOLEAN:
			{
				cmd.buffindex += sizeof(boolean);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((boolean*)(cmd.buff + cmd.buffindex)) = iter->second.data.b;
			}
			break;
				
			case _I16:
			{
				cmd.buffindex += sizeof(I16);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((I16*)(cmd.buff + cmd.buffindex)) = iter->second.data.i16;
			}
			break;

			case _I32:
			{
				cmd.buffindex += sizeof(I32);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((I32*)(cmd.buff + cmd.buffindex)) = iter->second.data.i32;
			}
			break;
			
			case _I64:
			{
				cmd.buffindex += sizeof(I64);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((I64*)(cmd.buff + cmd.buffindex)) = iter->second.data.i64;
			}
			break;

			case _U16:
			{
				cmd.buffindex += sizeof(U16);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((U16*)(cmd.buff + cmd.buffindex)) = iter->second.data.u16;
			}
			break;

			case _U32:
			{
				cmd.buffindex += sizeof(U32);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((U32*)(cmd.buff + cmd.buffindex)) = iter->second.data.u32;
			}
			break;
				
			case _U64:
			{
				cmd.buffindex += sizeof(U64);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((U64*)(cmd.buff + cmd.buffindex)) = iter->second.data.u64;
			}
			break;

			case _F32:
			{
				cmd.buffindex += sizeof(F32);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((F32*)(cmd.buff + cmd.buffindex)) = iter->second.data.f32;
			}
			
			case _F64:
			{
				cmd.buffindex += sizeof(F64);
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				*((F64*)(cmd.buff + cmd.buffindex)) = iter->second.data.f64;
			}
				
			case _String:
			{
				cmd.buffindex += sizeof(I32);
				int len = strlen(iter->second.data.str.str);
				cmd.buffindex += len;
				if (cmd.buffindex > cmd.bufflen){
					cmd.bufflen *= 2;
					cmd.resize(cmd.bufflen);
				}
				memcpy(cmd.buff + cmd.buffindex, iter->second.data.str.str, len);
			}
			break;

			default:
				break;
		}
	}
	len = cmd.buffindex;
}

} /* namespace comm_protocols */
} /* namespace Fossilizid */