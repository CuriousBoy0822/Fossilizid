/*
* qianqians
* 2014-9-11
*/
#include <WinSock2.h>

#include <list>
#include <vector>

#include <boost/atomic.hpp>
#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>

class UDPSession{
public:
	UDPSession();
	~UDPSession();

	boost::signals2::signal<void(char *, int) > sigRecv;

	boost::signals2::signal<void() > sigDisConnect;

	void disconnect();

	void reliable_send(char * buf, int len);

	void unreliable_send(char * buf, int len);

protected:
	void response(int serial, char cmdid);

	void complete(int serial, char cmdid);

protected:
	boost::mutex sendbufmutex;
	std::list<std::pair<boost::shared_ptr<char>, int > > sendbuf;

	boost::mutex unreliablesendbufmutex;
	std::list<std::pair<boost::shared_ptr<char>, int > > unreliablesendbuf;

	boost::mutex recvackbufmutex;
	std::map<boost::uint32_t, std::pair<boost::shared_ptr<char>, int > > recvackbuf;

	sockaddr_in remote_addr;

	boost::uint32_t index;
	boost::uint32_t serial;
	boost::uint32_t remote_serial;

	boost::atomic_int state;

	friend class UDPBase;
	friend class UDPService;

};