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
		objpool::bInit = false;

		if (!objpool::bInit){
			boost::mutex::scoped_lock l(objpool::mu);
			if (objpool::bInit){
				return
			}

			objpool::corenum = 8;
#ifdef _WINDOWS
			SYSTEM_INFO info;
			GetSystemInfo(&info);
			objpool::corenum = info.dwNumberOfProcessors;
#endif
			vpool.reserve(objpool::corenum);
			for (int i = 0; i < objpool::corenum; i++){
				vpool[i]._block = 0;
				vpool[i]._block8 = 0;
				vpool[i]._block16 = 0;
				vpool[i].recvlistchunk.clear();
			}
		}
	}

	~objpool(){
		for (int i = 0; i < objpool::corenum; i++){
			for (T * p = objpool::vpool[i]._block; p != 0;){
				T * tmp = p;
				p = *(T**(p));
				delete tmp;
			}
			objpool::vpool[i]._block = 0;

			for (T * p = objpool::vpool[i]._block8; p != 0;){
				T * tmp = p;
				p = *(T**(p));
				delete[] tmp;

 			}
			objpool::vpool[i]._block8 = 0;
			
			for (T * p = objpool::vpool[i]._block8; p != 0;){
				T * tmp = p;
				p = *(T**(p));
				delete[] tmp;
			}
			objpool::vpool[i]._block16 = 0;

			for (auto var : objpool::vpool[i].recvlistchunk){
				delete var.second;
			}
			objpool::vpool[i].recvlistchunk.clear();
		}
	}

public:
	static T * allocator(int len){
		T * ret = 0;
		for (int i = 0; i < objpool::corenum; i++){
			boost::mutex::scoped_try_lock l(objpool::vpool[i].mu);
			if (l.owns_lock()){
				if (len == 1){
					if (objpool::vpool[i]._block != 0){
						ret = objpool::vpool[i]._block;
						objpool::vpool[i]._block = *((T**)(objpool::vpool[i]._block));
					}
				}
				if (len > 1 && len <= 8){
					len = 8;
					if (objpool::vpool[i]._block != 0){
						ret = objpool::vpool[i]._block8;
						objpool::vpool[i]._block8 = *((T**)(objpool::vpool[i]._block8));
					}
				} else if (len > 8 && len <= 16){
					len = 16;
					if (objpool::vpool[i]._block != 0){
						ret = objpool::vpool[i]._block16;
						objpool::vpool[i]._block16 = *((T**)(objpool::vpool[i]._block16));
					}
				} else{
					len = (len + 15) / 16 * 16;
					if (!objpool::vpool[i].recvlistchunk.empty()){
						std::map<int, T * >::iterator it = objpool::vpool[i].recvlistchunk.lower_bound(len);
						ret = it->second;
						objpool::vpool[i].recvlistchunk.erase(it);
					}
				}
				
			}
		}

		if (ret == 0){
			ret = (T*)malloc(sizeof(T) * len);
		}
		
		return ret;
	}

	static void deallocator(T * buff, int len){
		T * ret = 0;
		int i = 0;
		while(1){
			boost::mutex::scoped_try_lock l(objpool::vpool[i].mu);
			if (l.owns_lock()){
				if (len == 1){
					buff->~T();
					*((T**)(buff)) = objpool::vpool[i]._block;
					objpool::vpool[i]._block = buff;
				}
				if (len > 1 && len <= 8){
					for (int _i = 0; _i < 8; _i++){
						buff->~T();
					}
					*((T**)(buff)) = objpool::vpool[i]._block8;
					objpool::vpool[i]._block8 = buff; 
				} else if (len > 8 && len <= 16){
					for (int _i = 0; _i < 16; _i++){
						buff->~T();
					}
					*((T**)(buff)) = objpool::vpool[i]._block16;
					objpool::vpool[i]._block16 = buff;
				} else{
					len = (len + 15) / 16 * 16;
					for (int _i = 0; _i < len; _i++){
						buff->~T();
					}
					objpool::vpool[i].recvlistchunk.insert(std::make_pair(len, buff));
				}
				break;
			}
			i += 1;
		}
	}

private:
	static std::vector<Tblock<T> > vpool;
	static boost::mutex mu;
	static bool bInit;
	static int corenum;

};

} /* namespace pool */
} /* namespace Fossilizid */

#endif //_objpool_h