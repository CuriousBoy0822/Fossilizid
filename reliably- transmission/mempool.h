/*
 * mempool.h
 *
 *  Created on: 2014-8-18
 *      Author: qianqians
 */
#ifndef _mempool_h
#define _mempool_h

#include <vector>
#include <map>
#include <boost/thread/mutex.hpp>

static int _4K = 4096;
static int _8K = 8192;
static int _16K = 16384;
static int _32K = 32768;
static int _64K = 65536;

namespace Fossilizid{
namespace comm_protocols {

struct block{
	char * buff;
	int len, index;
	char * recvlist4K;
	char * recvlist8K;
	char * recvlist16K;
	char * recvlist32K;
	char * recvlist64K;
	std::map<int, char * > recvlistchunk;
	boost::mutex mu;
};

class mempool{
public:
	mempool();
	~mempool();

public:
	static void * allocator(int len);
	static void deallocator(void * buff, int len);

private:
	static std::vector<block * > vpool;
	static int corenum;

};

} /* namespace comm_protocols */
} /* namespace Fossilizid */

#endif //_mempool_h