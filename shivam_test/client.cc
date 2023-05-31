#include "demo.h"
#include "commo.h"
#include "../src/rrr/rpc/client.hpp"
#include "../src/rrr/reactor/reactor.h"
#include<vector>
std::vector<demo::DemoProxy *> proxies;
std::vector<std::string> serverAddr;

void PopulateServerAddresses(){
	serverAddr.push_back("127.0.0.1:8090");
	serverAddr.push_back("127.0.0.1:8091");
	serverAddr.push_back("127.0.0.1:8092");
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
		proxies.push_back(new demo::DemoProxy(client.get()));
	}
	
	

	printf("checkpoint-1\n");
	// auto e = make_shared<ExampleQuorumEvent>(2, 2);
	// auto proxies = rpc_par_proxies_[par_id];
	// for (auto& p : proxies) {
	//   rrr::FutureAttr fuattr;
	//   fuattr.callback = [e](rrr::Future* fu) {
	//     e->FeedResponse(true);
	//   };
	// 	printf("checkpoint-2");
	// 	rrr::Future::safe_release(p->async_hello(hi, fuattr));
	// }
	// printf("waiting for the event\n");
	// e->Wait();
	// printf("checkpoint-3");
	// if (e->IsReady()){
	// 	printf("got a quorum\n");
	// }

	for (int i=0; i<3; i++){
	std::string hi("Hello Server" + std::to_string(i));
	std::string reply;
	
	proxies[i]->hello(hi, &reply);
	std::cout<<reply.c_str() << "\n";
	}
	// rrr::i32 a = 1, b = 2, c = 3;
	// rrr::i32 result;

	// client_proxy->sum(a, b, c, &result);
	// std::cout<<"sum result: " << result  << "\n";

}
