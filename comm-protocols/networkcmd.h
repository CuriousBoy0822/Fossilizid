/*
 * networkobj.h
 *
 *  Created on: 2014-8-18
 *      Author: qianqians
 */
#ifndef _networkobj_h
#define _networkobj_h

#include <map>
#include "enumtype.h"
#include "vector.h"

namespace Fossilizid{
namespace comm_protocols {

class cmd{
public:
	cmd();
	~cmd();
	
	cmd(const cmd & _cmd);
	cmd & operator = (const cmd & _cmd);

public:
	network_type type(String key);

	void insert(const boolean & value, String key);
	void insert(const I16 & value, String key);
	void insert(const I32 & value, String key);
	void insert(const I64 & value, String key);
	void insert(const U16 & value, String key);
	void insert(const U32 & value, String key);
	void insert(const U64 & value, String key);
	void insert(const F32 & value, String key);
	void insert(const F64 & value, String key);
	void insert(const String & value, String key);
	void insert(const vector & v, String key);
	void insert(const obj & v, String key);

	bool get(boolean & value, String key);
	bool get(I16 & value, String key);
	bool get(I32 & value, String key);
	bool get(I64 & value, String key);
	bool get(U16 & value, String key);
	bool get(U32 & value, String key);
	bool get(U64 & value, String key);
	bool get(F32 & value, String key);
	bool get(F64 & value, String key);
	bool get(String & value, String key);
	bool get(vector & v, String key);
	bool get(obj & v, String key);

private:
	std::map<String, elementinfo> mapelement;

private:
	friend std::pair<const char * , const int> makecmdtobuf(const cmd & cmd);
	friend const cmd makebuftocmd(char * buf, int len);

};

std::pair<const char * , const int> makecmdtobuf(const cmd & cmd);
const cmd makebuftocmd(const char * const buf, const int len);

} /* namespace comm_protocols */
} /* namespace Fossilizid */

#endif //_networkobj_h