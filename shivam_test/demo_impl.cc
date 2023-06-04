#include "demo_impl.h"
#include "../src/rrr/rpc/client.hpp"
#include "../src/rrr/reactor/reactor.h"
#include "test_command.h"
#include<vector>


namespace janus{

	DemoServiceImpl::DemoServiceImpl(rrr::Server* s, std::vector<std::string> serverAddr)
    : server_(s), serverAddr_(serverAddr){
		Log_info("constructor of DemoServiceImpl called\n");
	}

	// Communicator* DemoServiceImpl::findOrCreateCommo(){
	// 	if (!commo_){
	// 		commo_ = new demo::Communicator(serverAddr_);
	// 	}
	// 	return commo_;
	// }

	void DemoServiceImpl::populateProxies(){
		rrr::PollMgr *pm = new rrr::PollMgr();
		for (int i = 0; i < serverAddr_.size(); i++)
		{
			std::shared_ptr<rrr::Client> client = std::make_shared<rrr::Client>(pm);
			auto connect_result = client->connect(serverAddr_[i].c_str(), false);
			if (connect_result != 0)
			{
				Log_info("connection unsuccessful");
			}
			else
			{
				Log_info("connection successful");
			}
			// Log_info("checkpoint-0\n");
			proxies_[serverAddr_[i]] = new janus::DemoProxy(client.get());
		}

		// test if proxies_ got it right
		if(proxies_.find(serverAddr_[0]) != proxies_.end()){
			Log_info("found proxy for first server");
		}
		else{
			Log_info("issue finding proxy for first server");
		}

	}

	void DemoServiceImpl:: OnHello(){
		Log_info("inside demoserviceimpl::hello in %s", server_->addr_.c_str());
	}

	void DemoServiceImpl::hello(const std::string& hi, std::string* reply, rrr::DeferredReply* defer){
		Log_info("inside demoserviceimpl::hello in %s", server_->addr_.c_str());
		Log_info("server add is: %s\n", server_->addr_.c_str());

		if (proxies_.size()==0){
			populateProxies();
		}

		auto it = std::find(serverAddr_.begin(), serverAddr_.end(), server_->addr_);
		if(it != serverAddr_.end()){
			int index = it - serverAddr_.begin();
			if (index < serverAddr_.size()-1){
				std::string testString("hello_from_first");
				auto cmd = std::make_shared<TestCommand>();
				cmd->hi = hi;
				auto sp_m = dynamic_pointer_cast<Marshallable>(cmd);
				MarshallDeputy md(sp_m);
				Log_info("**** inside demoserviceimpl::hello; calling cRPC");
				proxies_[serverAddr_[index+1]]->cRPC(md, serverAddr_);
			}
		}



		// std::string hi2("Hello Server 1 from Server 0");
		// std::string reply2;
		// proxies[0]->hello(hi2, &reply2);
		// std::cout << reply2.c_str() << "\n";

		// *reply += std::string("Re123: ") + hi + " adding reply from server 1: " + reply2.c_str();


		// // original code
		*reply += std::string("Re123: ") + hi;
		defer->reply(); // commenting here, moved to wrapper for testing
		Log_info("returning demoserviceimpl::hello in %s", server_->addr_.c_str());
	}

	void DemoServiceImpl::sum(const rrr::i32& a, const rrr::i32& b, const rrr::i32& c, rrr::i32* result, rrr::DeferredReply* defer){
		Log_info("inside demoserviceimpl::sum in %s", server_->addr_.c_str());
		if (proxies_.size()==0){
			populateProxies();
		}

		*result = a + b + c;
		defer->reply();
		Log_info("returning demoserviceimpl::sum in %s", server_->addr_.c_str());
	}

	void DemoServiceImpl::cRPC(const janus::MarshallDeputy& cmd, const std::vector<std::string>& addrChain, rrr::DeferredReply* defer){
		Log_info("inside demoserviceimpl::cRPC in %s", server_->addr_.c_str());
		Log_info("inside demoserviceimpl::cRPC; cmd type is:%d", cmd.kind_); 
		// //check the cmd.kind_ to know which upper function to call
		auto c = dynamic_pointer_cast<TestCommand>(cmd.sp_data_);
		Log_info("inside demoserviceimpl::cRPC; cmd message is: %s", c->hi.c_str());

		if (proxies_.size()==0){
			populateProxies();
		}
		for (auto el : addrChain){
			Log_info("server add in the chain is: %s\n", el.c_str());
		}



		// below is the chain formation with just chain calls
		// auto it = std::find(addrChain.begin(), addrChain.end(), server_->addr_);
		// if(it != addrChain.end()){
		// 	Log_info("demoserviceimpl::cRPC; checkpoint 0 in %s", server_->addr_.c_str());
		// 	int index = it - addrChain.begin();
		// 	if (index < addrChain.size()-1){
		// 		Log_info("demoserviceimpl::cRPC; checkpoint 1 in %s", server_->addr_.c_str());
		// 		std::string testString("hello_from_first");
		// 		Log_info("demoserviceimpl::cRPC; checkpoint 2 in %s, index is: %d", server_->addr_.c_str(), index+1);
		// 		Log_info("demoserviceimpl::cRPC; checkpoint 3 in %s, address is: %s", server_->addr_.c_str(), addrChain[index+1].c_str());
		// 		proxies_[addrChain[index+1]]->cRPC(md, addrChain);
		// 	}
		// }

		auto it = std::find(serverAddr_.begin(), serverAddr_.end(), server_->addr_);
		if(it != serverAddr_.end()){
			int index = it - serverAddr_.begin();
			if (index < serverAddr_.size()-1){
				std::string testString("hello_from_first");
				auto cmd = std::make_shared<TestCommand>();
				cmd->hi = c->hi.c_str();
				auto sp_m = dynamic_pointer_cast<Marshallable>(cmd);
				MarshallDeputy md(sp_m);
				Log_info("**** inside demoserviceimpl::hello; calling cRPC");
				proxies_[serverAddr_[index+1]]->cRPC(md, serverAddr_);
			}
		}

		Log_info("demoserviceimpl::cRPC; checkpoint 3 in %s", server_->addr_.c_str());
		defer->reply();
		Log_info("returning demoserviceimpl::cRPC in %s", server_->addr_.c_str());
	}
}
