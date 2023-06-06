#include "demo.h"
#include "commo.h"
#include "../src/rrr/rpc/client.hpp"
#include "../src/rrr/reactor/reactor.h"
#include<vector>
std::vector<janus::DemoProxy *> proxies;
std::vector<std::string> serverAddr;

void PopulateServerAddresses(){
	serverAddr.push_back("127.0.0.1:8091");
	serverAddr.push_back("127.0.0.1:8092");
	serverAddr.push_back("127.0.0.1:8093");
}


int main(){
	rrr::PollMgr *pm = new rrr::PollMgr();
	
	// get the server addresses
	PopulateServerAddresses();
	for (int i=0; i<serverAddr.size(); i++){
		std::shared_ptr<rrr::Client> client = std::make_shared<rrr::Client>(pm);
		auto connect_result = client->connect(serverAddr[i].c_str(), false);	
		if(connect_result != 0){
			printf("connection unsuccessful\n");
			return -1;
		}
		else{
			printf("connection successful\n");
		}
		printf("checkpoint-0\n");
		proxies.push_back(new janus::DemoProxy(client.get()));
	}
	
	

	printf("checkpoint-1\n");


	// broadcast request to all servers
	// for (int i = 0; i < 3; i++)
	// {
	// 	std::string hi("Hello Server" + std::to_string(i));
	// 	std::string reply;

	// 	proxies[i]->hello(hi, &reply);
	// 	std::cout << reply.c_str() << "\n";
	// }

	// just send request to one proxy to check if chain may happen
	std::string hi("Hello Server" + std::to_string(0));
	std::string reply;
	proxies[0]->hello(hi, &reply);
	std::cout << reply.c_str() << "\n";

	// rrr::i32 a = 1, b = 2, c = 3;
	// rrr::i32 result;

	// client_proxy->sum(a, b, c, &result);
	// std::cout<<"sum result: " << result  << "\n";

}
