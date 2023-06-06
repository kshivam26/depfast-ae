#include "commo.h"
#include "../src/rrr/rpc/client.hpp"
#include "../src/rrr/reactor/reactor.h"


namespace janus {
    Communicator::Communicator(std::vector<std::string> serverAddr){
        rrr::PollMgr *pm = new rrr::PollMgr();
		for (int i = 0; i < serverAddr.size(); i++)
		{
			std::shared_ptr<rrr::Client> client = std::make_shared<rrr::Client>(pm);
			auto connect_result = client->connect(serverAddr[i].c_str(), false);
			if (connect_result != 0)
			{
				printf("connection unsuccessful\n");
			}
			else
			{
				printf("connection successful\n");
			}
			printf("checkpoint-0\n");
			proxies_[serverAddr[i]] = new janus::DemoProxy(client.get());
		}

		// test if proxies_ got it right
		if(proxies_.find(serverAddr[0]) != proxies_.end()){
			printf("found proxy for first server");
		}
		else{
			printf("issue finding proxy for first server");
		}
    }
} // namespace demo


// class Communicator {
//     public:
//       std::map<std::string, demo::DemoProxy *> proxies_;
//   };