#include <vector>
#include "../../reliablyt/UDPService.h"
#include "../../reliablyt/UDPConnect.h"

std::vector<boost::shared_ptr<UDPConnect> > vmap;

void onRecv(char * buf, int len){
	char formatbuf[32];
	memset(formatbuf, 0, 32);
	memcpy(formatbuf, buf, len);
	printf(formatbuf);
	printf("\n");
}

void onConnect(boost::shared_ptr<UDPConnect> c){
	vmap.push_back(c);
	c->sigRecv.connect(onRecv);
}

int main(){
	UDPService _service("127.0.0.1", 7777);

	_service.sigConnect.connect(onConnect);

	while (1){
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		for (auto v : vmap){
			v->reliable_send("test server", 11);
		}
	}

	return 0;
}