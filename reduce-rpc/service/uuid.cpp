/*
 * uuid.cpp
 *
 *  Created on: 2014-10-15
 *      Author: qianqians
 */
#include "uuid.h"

namespace Fossilizid{
namespace reduce_rpc {

uuid UUID(){
	std::string _uuid;
	_uuid.resize(16);

	{
		int t = (int)time(0);

		_uuid[0] = t & 0xff;
		_uuid[1] = (t & 0xff00) >> 8;
		_uuid[2] = (t & 0xff0000) >> 16;
		_uuid[3] = (t & 0xff000000) >> 24;
	}

	{
#ifdef _WINDOWS
		static IP_ADAPTER_INFO info;
		static bool isinit = false;
		ULONG size = sizeof(IP_ADAPTER_INFO);
		if (!isinit){
			if (GetAdaptersInfo(&info, &size) == ERROR_SUCCESS){
				isinit = true;
			}
		}

		_uuid[4] = info.Address[0];
		_uuid[5] = info.Address[1];
		_uuid[6] = info.Address[2];
		_uuid[7] = info.Address[3];
		_uuid[8] = info.Address[4];
		_uuid[9] = info.Address[5];

		static DWORD id = GetCurrentProcessId();

		_uuid[10] = (char)(id & 0xff);
		_uuid[11] = (char)((id & 0xff00) >> 8);
		_uuid[12] = (char)((id & 0xff0000) >> 16);
		_uuid[13] = (char)((id & 0xff000000) >> 24);

#endif
	}

	static short id = 0;

	{
		_uuid[14] = id & 0xff;
		_uuid[15] = (id & 0xff00) >> 8;
	}

	return _uuid;
}

} /* namespace reduce_rpc */
} /* namespace Fossilizid */