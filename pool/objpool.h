/*
 * objpool.h
 *
 *  Created on: 2014-8-18
 *      Author: qianqians
 */
#ifndef _objpool_h
#define _objpool_h

#include <vector>
#include <map>
#include <boost/thread/mutex.hpp>

#ifdef _WINDOWS
#include <Windows.h>
#endif

namespace Fossilizid{
namespace pool{

template<typename T>
struct Tblock{
	T * _block;
	T * _block8;
	T * _block16;
	std::map<int, T * > recvlistchunk;
	boost::mutex mu;
};

template<typename T>
class objpool{
public:
	objpool(){
		bInit = false;

		if (!bInit){
			boost::mutex::scoped_lock l(mu);
			if (bInit){
				return;
			}

			corenum = 8;
#ifdef _WINDOWS
			SYSTEM_INFO info;
			GetSystemInfo(&info);
			corenum = info.dwNumberOfProcessors;
#endif
			_vpool.resize(corenum);
			for (int i = 0; i < corenum; i++){
				_vpool[i] = new Tblock<T>;
				_vpool[i]->_block = 0;
				_vpool[i]->_block8 = 0;
				_vpool[i]->_block16 = 0;
				_vpool[i]->recvlistchunk.clear();
			}
		}
	}

	~objpool(){
		for (int i = 0; i < corenum; i++){
			for (T * p = _vpool[i]->_block; p != 0;){
				T * tmp = p;
				p = *(T**(p));
				delete tmp;
			}
			_vpool[i]->_block = 0;

			for (T * p = _vpool[i]->_block8; p != 0;){
				T * tmp = p;
				p = *(T**(p));
				delete[] tmp;

 			}
			_vpool[i]->_block8 = 0;
			
			for (T * p = _vpool[i]->_block8; p != 0;){
				T * tmp = p;
				p = *(T**(p));
				delete[] tmp;
			}
			_vpool[i]->_block16 = 0;

			for (auto var : _vpool[i]->recvlistchunk){
				delete var.second;
			}
			_vpool[i]->recvlistchunk.clear();

			for (int i = 0; i < corenum; i++){
				delete _vpool[i];
			}
		}
	}

public:
	static void Init(){
		phandle = new objpool;
	}

	static T * allocator(int len){
		T * ret = 0;
		for (int i = 0; i < phandle->corenum; i++){
			boost::mutex::scoped_try_lock l(phandle->_vpool[i]->mu);
			if (l.owns_lock()){
				if (len == 1){
					if (phandle->_vpool[i]->_block != 0){
						ret = phandle->_vpool[i]->_block;
						phandle->_vpool[i]->_block = *((T**)((void*)ret));
					}
				}
				if (len > 1 && len <= 8){
					len = 8;
					if (phandle->_vpool[i]->_block8 != 0){
						ret = phandle->_vpool[i]->_block8;
						phandle->_vpool[i]->_block8 = *((T**)((void*)phandle->_vpool[i]->_block8));
					}
				} else if (len > 8 && len <= 16){
					len = 16;
					if (phandle->_vpool[i]->_block16 != 0){
						ret = phandle->_vpool[i]->_block16;
						phandle->_vpool[i]->_block16 = *((T**)((void*)phandle->_vpool[i]->_block16));
					}
				} else{
					len = (len + 15) / 16 * 16;
					if (!phandle->_vpool[i]->recvlistchunk.empty()){
						std::map<int, T * >::iterator it = phandle->_vpool[i]->recvlistchunk.lower_bound(len);
						ret = it->second;
						phandle->_vpool[i]->recvlistchunk.erase(it);
					}
				}
			}
		}

		if (ret == 0){
			ret = (T*)malloc((sizeof(T)+7)/8*8*len);
		}
		
		return ret;
	}

	static void deallocator(T * buff, int len){
		T * ret = 0;
		int i = 0;
		while(1){
			boost::mutex::scoped_try_lock l(phandle->_vpool[i]->mu);
			if (l.owns_lock()){
				if (len == 1){
					buff->~T();
					memset(buff, 0, sizeof(buff));
					*((void**)((void*)buff)) = (void*)phandle->_vpool[i]->_block;
					phandle->_vpool[i]->_block = buff;
				}
				if (len > 1 && len <= 8){
					for (int _i = 0; _i < 8; _i++){
						buff->~T();
					}
					memset(buff, 0, sizeof(buff));
					*((void**)((void*)buff)) = (void*)phandle->_vpool[i]->_block8;
					phandle->_vpool[i]->_block8 = buff;
				} else if (len > 8 && len <= 16){
					for (int _i = 0; _i < 16; _i++){
						buff->~T();
					}
					memset(buff, 0, sizeof(buff));
					*((void**)((void*)buff)) = (void*)phandle->_vpool[i]->_block16;
					phandle->_vpool[i]->_block16 = buff;
				} else{
					len = (len + 15) / 16 * 16;
					for (int _i = 0; _i < len; _i++){
						buff->~T();
					}
					memset(buff, 0, sizeof(buff));
					phandle->_vpool[i]->recvlistchunk.insert(std::make_pair(len, buff));
				}
				break;
			}
			
			if (++i == phandle->corenum){
				i = 0;
			}
		}
	}

private:
	static objpool * phandle;

	std::vector<Tblock<T> * > _vpool;
	bool bInit;
	int corenum;

};

template<typename T>
objpool<T> * objpool<T>::phandle;

} /* namespace pool */
} /* namespace Fossilizid */

#endif //_objpool_h