#include "demo.h"
#include "commo.h"
#include "../src/rrr/rpc/client.hpp"
#include "../src/rrr/reactor/reactor.h"
#include<vector>

int main(){
	rrr::PollMgr *pm = new rrr::PollMgr();
	std::shared_ptr<rrr::Client> client = std::make_shared<rrr::Client>(pm);
	while(client->connect(std::string("127.0.0.1:8090").c_str())!=0){
		usleep(100*100);
	}

	printf("checkpoint-0\n");
	// auto e = rrr::Reactor::CreateSpEvent<ExampleQuorumEvent>(1,1);
	std::vector<demo::DemoProxy *> proxies;
	auto client_proxy = new demo::DemoProxy(client.get());
	// proxies.push_back(new demo::DemoProxy(client.get()));
	// proxies.push_back(new demo::DemoProxy(client.get()));

	std::string hi = "Hello Server1";
	std::string reply;

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

	

	client_proxy->hello(hi, &reply);
	std::cout<<reply.c_str() << "\n";

	// rrr::i32 a = 1, b = 2, c = 3;
	// rrr::i32 result;

	// client_proxy->sum(a, b, c, &result);
	// std::cout<<"sum result: " << result  << "\n";

}
