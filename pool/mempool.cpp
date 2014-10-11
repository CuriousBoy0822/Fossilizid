/*
 * mempool.h
 *
 *  Created on: 2014-8-18
 *      Author: qianqians
 */
#include "mempool.h"

#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace Fossilizid{
namespace pool {

mempool __mempool__;

std::vector<block * > mempool::vpool;
int mempool::corenum;

static int chunck_normal_len = 1048576;

static mempool pool; 
static bool bInit = false;
static boost::mutex mu;

static int _4K = 4096;
static int _8K = 8192;
static int _16K = 16384;
static int _32K = 32768;
static int _64K = 65536;

int standardization_lenght(int len){
	if (len <= _4K){
		len = _4K;
	} else if (len <= _8K){
		len = _8K;
	} else if (len <= _16K){
		len = _16K;
	} else if (len <= _32K){
		len = _32K;
	} else if (len <= _64K){
		len = _64K;
	} else{
		len = (len + _4K - 1) / _4K * _4K;
	}

	return len;
}

mempool::mempool(){
	if (bInit == false){
		boost::mutex::scoped_lock lock(mu);
		if (bInit == true){
			return;
		}

		corenum = 8;
#ifdef _WINDOWS
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		corenum = info.dwNumberOfProcessors;
#endif
		vpool.reserve(corenum);
		for (int i = 0; i < corenum; i++){
			vpool[i] = new block;
			vpool[i]->buff = new char[chunck_normal_len];
			vpool[i]->len = chunck_normal_len;
			vpool[i]->index = 0;
			vpool[i]->recvlist4K = 0;
			vpool[i]->recvlist8K = 0;
			vpool[i]->recvlist16K = 0;
			vpool[i]->recvlist32K = 0;
			vpool[i]->recvlist64K = 0;
		}
	}
}

mempool::~mempool(){
	if (bInit == false){
		boost::mutex::scoped_lock lock(mu);
		if (bInit == true){
			return;
		}

		for (int i = 0; i < corenum; i++){
			delete vpool[i]->buff;
			delete vpool[i];
		}
	}
}

void * mempool::allocator(int len){
	void * mem = 0;
	for (int index = 0; index < corenum; index++){
		boost::mutex::scoped_try_lock lock(vpool[index]->mu);
		if (lock.owns_lock()){
			if (len < (vpool[index]->len - vpool[index]->index)){
				mem = vpool[index]->buff + vpool[index]->index;
				vpool[index]->index += len;
			} else{
				if (len < _4K && vpool[index]->recvlist4K != 0 && mem == 0){
					mem = vpool[index]->recvlist4K;
					vpool[index]->recvlist4K = *((char**)mem);
				}

				if (len < _8K && vpool[index]->recvlist8K != 0 && mem == 0){
					mem = vpool[index]->recvlist8K;
					vpool[index]->recvlist8K = *((char**)mem);
				}

				if (len < _16K && vpool[index]->recvlist16K != 0 && mem == 0){
					mem = vpool[index]->recvlist16K;
					vpool[index]->recvlist16K = *((char**)mem);
				}

				if (len < _32K && vpool[index]->recvlist32K != 0 && mem == 0){
					mem = vpool[index]->recvlist32K;
					vpool[index]->recvlist32K = *((char**)mem);
				}

				if (len < _64K && vpool[index]->recvlist64K != 0 && mem == 0){
					mem = vpool[index]->recvlist64K;
					vpool[index]->recvlist64K = *((char**)mem);
				}

				if (mem == 0){
					std::map<int, char * >::iterator find = vpool[index]->recvlistchunk.lower_bound(len);
					if (find != vpool[index]->recvlistchunk.end()){
						mem = find->second;
					}
				}
			}

			if (mem != 0){
				break;
			}
		}
	}

	if (mem == 0){
		int tmplen = 0;
		if (len <= _4K){
			tmplen = _4K;
		} else if (len <= _8K){
			tmplen = _8K;
		} else if (len <= _16K){
			tmplen = _16K;
		} else if (len <= _32K){
			tmplen = _32K;
		} else if (len <= _64K){
			tmplen = _64K;
		} else{
			tmplen = (len + _4K - 1) / _4K * _4K;
		}
		mem = new char[tmplen];
	}

	return mem;
}

void mempool::deallocator(void * buff, int len){
	int index = 0;
	while (1){
		if (len < _4K){
			*((char**)buff) = vpool[index]->recvlist4K;
			vpool[index]->recvlist4K = (char*)buff;
		}else if (len < _8K){
			*((char**)buff) = vpool[index]->recvlist8K;
			vpool[index]->recvlist8K = (char*)buff;
		}else if (len < _16K){
			*((char**)buff) = vpool[index]->recvlist16K;
			vpool[index]->recvlist16K = (char*)buff;
		}else if (len < _32K){
			*((char**)buff) = vpool[index]->recvlist32K;
			vpool[index]->recvlist32K = (char*)buff;
		}else if (len < _64K){
			*((char**)buff) = vpool[index]->recvlist64K;
			vpool[index]->recvlist64K = (char*)buff;
		}else {
			int tmplen = 0;
			if (len <= _4K){
				tmplen = _4K;
			} else if (len <= _8K){
				tmplen = _8K;
			} else if (len <= _16K){
				tmplen = _16K;
			} else if (len <= _32K){
				tmplen = _32K;
			} else if (len <= _64K){
				tmplen = _64K;
			} else{
				tmplen = (len + _4K - 1) / _4K * _4K;
			}
			vpool[index]->recvlistchunk.insert(std::make_pair(tmplen, (char*)(buff)));
		}
	}
}	

} /* namespace pool */
} /* namespace Fossilizid */