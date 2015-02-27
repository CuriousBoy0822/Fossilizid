#include "../../reliablyt/UDPClient.h"

void onRecv(char * buf, int len){
	char formatbuf[32];
	memset(formatbuf, 0, 32);
	memcpy(formatbuf, buf, len);
	printf(formatbuf);
	printf("\n");
}

int main(){
	UDPClient _client("127.0.0.1", 0);

	_client.connect("127.0.0.1", 7777);

	_client.sigRecv.connect(onRecv);

	while (1){
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		_client.reliable_send("test client", 11);
	}

	return 0;
}