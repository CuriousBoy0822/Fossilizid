#include "../juggle_achieve/channelserver.h"
#include "../juggle_scprit/server/jugglemodule.h"

class juggleimpl : public juggle{
public:
	virtual std::string login(std::string argv3){
		printf("juggleimpl login %s\n", argv3.c_str());

		return "login sucess";
	}

	virtual std::string test(std::string argv3){
		printf("juggleimpl test %s\n", argv3.c_str());

		return "test sucess";
	}

};

juggle * create_juggle(){
	return new juggleimpl();
}

int main(){
	Fossilizid::reduce::acceptor::channelserver server(Fossilizid::juggle::create_service());

	server.init("127.0.0.1", 1234);

	while (1){
		server.poll();
	}

	return 1;
}
