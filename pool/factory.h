/*
 * factory.h
 *
 *  Created on: 2014-10-21
 *      Author: qianqians
 */
#ifndef _factory_h
#define _factory_h

#include "mempool.h"

namespace Fossilizid{
namespace pool{

class factory{
public:
	factory();
	~factory();

public:
	template<class T, typename ...Tlist>
	static T * create(int count, Tlist&& ... var){
		T * p = (T*)mempool::allocator(count * sizeof(T));

		for (int i = 0; i < count; i++){
			new (&p[i]) T(std::forward<Tlist>(var)...);
		}

		return p;
	}

	template<class T, typename ...Tlist>
	static T * create(Tlist&& ... var){
		T * p = mempool::allocator(sizeof(T));

		new(p) T(std::forward<Tlist>(var)...);

		return p;
	}

	template<class T>
	static void release(T * p, int count){
		for (int i = 0; i < count; i++){
			p[i].~T();
		}

		mempool::deallocator(p, count*sizeof(T));
	}

};

} /* namespace pool */
} /* namespace Fossilizid */

#endif //_factory_h