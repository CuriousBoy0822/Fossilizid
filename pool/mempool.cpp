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

std::vector<block * > mempool::vpool;
int mempool::corenum;

mempool __mempool__;

static int chunck_normal_len = 1048576;

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

		mempool::corenum = 8;
#ifdef _WINDOWS
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		mempool::corenum = info.dwNumberOfProcessors;
#endif
		mempool::vpool.resize(mempool::corenum);
		for (int i = 0; i < mempool::corenum; i++){
			mempool::vpool[i] = new block;
			mempool::vpool[i]->buff = new char[chunck_normal_len];
			mempool::vpool[i]->len = chunck_normal_len;
			mempool::vpool[i]->index = 0;
			mempool::vpool[i]->recvlist4K = 0;
			mempool::vpool[i]->recvlist8K = 0;
			mempool::vpool[i]->recvlist16K = 0;
			mempool::vpool[i]->recvlist32K = 0;
			mempool::vpool[i]->recvlist64K = 0;
		}
	}
}

mempool::~mempool(){
	if (bInit == false){
		boost::mutex::scoped_lock lock(mu);
		if (bInit == true){
			return;
		}

		for (int i = 0; i < mempool::corenum; i++){
			delete mempool::vpool[i]->buff;
			delete mempool::vpool[i];
		}
		mempool::vpool.clear();
	}
}

void * mempool::allocator(int len){
	void * mem = 0;
	for (int index = 0; index < mempool::corenum; index++){
		boost::mutex::scoped_try_lock lock(mempool::vpool[index]->mu);
		if (lock.owns_lock()){
			if (len < (mempool::vpool[index]->len - mempool::vpool[index]->index)){
				mem = mempool::vpool[index]->buff + mempool::vpool[index]->index;
				mempool::vpool[index]->index += len;
			} else{
				if (len < _4K && mempool::vpool[index]->recvlist4K != 0 && mem == 0){
					mem = mempool::vpool[index]->recvlist4K;
					mempool::vpool[index]->recvlist4K = *((char**)mem);
				}

				if (len < _8K && mempool::vpool[index]->recvlist8K != 0 && mem == 0){
					mem = mempool::vpool[index]->recvlist8K;
					mempool::vpool[index]->recvlist8K = *((char**)mem);
				}

				if (len < _16K && mempool::vpool[index]->recvlist16K != 0 && mem == 0){
					mem = mempool::vpool[index]->recvlist16K;
					mempool::vpool[index]->recvlist16K = *((char**)mem);
				}

				if (len < _32K && mempool::vpool[index]->recvlist32K != 0 && mem == 0){
					mem = mempool::vpool[index]->recvlist32K;
					mempool::vpool[index]->recvlist32K = *((char**)mem);
				}

				if (len < _64K && mempool::vpool[index]->recvlist64K != 0 && mem == 0){
					mem = mempool::vpool[index]->recvlist64K;
					mempool::vpool[index]->recvlist64K = *((char**)mem);
				}

				if (mem == 0){
					std::map<int, char * >::iterator find = mempool::vpool[index]->recvlistchunk.lower_bound(len);
					if (find != mempool::vpool[index]->recvlistchunk.end()){
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
		mem = new char[standardization_lenght(len)];
	}

	return mem;
}

void mempool::deallocator(void * buff, int len){
	int index = 0;
	while (1){
		boost::mutex::scoped_try_lock lock(mempool::vpool[index]->mu);
		if (lock.owns_lock()){
			if (len < _4K){
				*((char**)buff) = mempool::vpool[index]->recvlist4K;
				mempool::vpool[index]->recvlist4K = (char*)buff;
			} else if (len < _8K){
				*((char**)buff) = mempool::vpool[index]->recvlist8K;
				mempool::vpool[index]->recvlist8K = (char*)buff;
			} else if (len < _16K){
				*((char**)buff) = mempool::vpool[index]->recvlist16K;
				mempool::vpool[index]->recvlist16K = (char*)buff;
			} else if (len < _32K){
				*((char**)buff) = mempool::vpool[index]->recvlist32K;
				mempool::vpool[index]->recvlist32K = (char*)buff;
			} else if (len < _64K){
				*((char**)buff) = mempool::vpool[index]->recvlist64K;
				mempool::vpool[index]->recvlist64K = (char*)buff;
			} else {
				mempool::vpool[index]->recvlistchunk.insert(std::make_pair(standardization_lenght(len), (char*)(buff)));
			}
			break;
		}

		if (++index == mempool::corenum){
			index = 0;
		}
	}
}	

} /* namespace pool */
} /* namespace Fossilizid */