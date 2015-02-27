#include "../juggle_achieve/channelserver.h"
#include "../juggle_scprit/client/jugglecaller.h"

int main(){
	Fossilizid::reduce::connect::channelserver server(Fossilizid::juggle::create_service());
	
	server.init();

	boost::shared_ptr<Fossilizid::juggle::channel> ch = server.connect("127.0.0.1", 1234);

	sync::juggle j(ch.get());

	std::cout << j.login("i am login").c_str() << std::endl;
	std::cout << j.test("i am test").c_str() << std::endl;

	while(1){
		server.poll();
	}

	return 1;
}
