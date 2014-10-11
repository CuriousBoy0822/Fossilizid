/*
 *  qianqians
 *  2014-10-5
 */
#include "../../container/msque.h"
#include "../../container/optimisticque.h"
#include "../../container/ringque.h"
#include "../../container/swapque.h"
#include "../../container/small_hash_map.h"

#include <queue>
#include <boost/thread.hpp>

#include <time.h>

struct em{
	em(){};

	em(int _tid, int _i){
		tid = _tid;
		i = _i;
	}

	int tid;
	int i;
};

boost::mutex mu;
std::queue<em> que;
std::vector<std::pair<boost::mutex *, std::queue<int> > > vque;

template<class que>
void push(int tid, que * _que, int i){
	_que->push(em(tid, i));
}

void push_stl(int tid, std::queue<em> * _que, int i){
	boost::mutex::scoped_lock lock(mu);
	_que->push(em(tid, i));
}

template<class que>
void pop(int tid, que * _que){
	while (1)
	{
		em _em;

		{
			if (!_que->pop(_em)){
				continue;
			}
		}

		boost::mutex::scoped_lock l(*vque[_em.tid].first);
		vque[_em.tid].second.push(_em.i);

		if (vque[0].second.size() == 100000 &&
			vque[1].second.size() == 100000 &&
			vque[2].second.size() == 100000 &&
			vque[3].second.size() == 100000)
		{
			break;
		}
	}
}

void pop_stl(int tid, std::queue<em> * que){
	while (1)
	{
		em _em;

		{
			boost::mutex::scoped_lock lock(mu);
			if (que->empty()){
				continue;
			}

			_em = que->front();
			que->pop();
		}

		boost::mutex::scoped_lock l(*vque[_em.tid].first);
		vque[_em.tid].second.push(_em.i);

		if (vque[0].second.size() == 100000 &&
			vque[1].second.size() == 100000 &&
			vque[2].second.size() == 100000 &&
			vque[3].second.size() == 100000)
		{
			break;
		}
	}
}

void do_test_stl(int tid, std::queue<em> * que){
	for (int i = 0; i < 100000; i++){
		push_stl(tid, que, i);
	}
}

template<class que>
void do_test(int tid, que * _que){
	for (int i = 0; i < 100000; i++){
		push(tid, _que, i);
	}
}

int main(){
	clock_t begin = clock();
	boost::thread_group _group;

	{
		vque.resize(4);
		for (int i = 0; i < 4; i++)
		{
			vque[i].first = new boost::mutex;
		}

		begin = clock();

		for (int i = 0; i < 4; i++){
			_group.create_thread(boost::bind(do_test_stl, i, &que));
		}
		_group.create_thread(boost::bind(pop_stl, 0, &que));
		_group.join_all();

		for (auto v : vque){
			if (v.second.size() != 100000){
				printf("size error\n");
			}

			for (int i = 0; i < 100000; i++){
				if (v.second.front() != i){
					printf("push error\n");
				}
				v.second.pop();
 			}
		}

		printf("std::queue time %d\n", clock() - begin);

		vque.clear();
	}

	{
		vque.resize(4);
		for (int i = 0; i < 4; i++)
		{
			vque[i].first = new boost::mutex;
		}

		begin = clock();

		Fossilizid::container::msque<em> que;

		for (int i = 0; i < 4; i++){
			_group.create_thread(boost::bind(do_test<Fossilizid::container::msque<em>>, i, &que));
		}
		_group.create_thread(boost::bind(pop<Fossilizid::container::msque<em>>, 0, &que));
		_group.join_all();

		for (auto v : vque){
			if (v.second.size() != 100000){
				printf("size error\n");
			}

			for (int i = 0; i < 100000; i++){
				if (v.second.front() != i){
					printf("push error\n");
				}
				v.second.pop();
			}
		}

		printf("msque time %d\n", clock() - begin);

		vque.clear();
	}

	return 0;
}