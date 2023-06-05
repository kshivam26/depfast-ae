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
		for (int i = 0; i < serverAddr_.size(); i++)
		{
			rrr::PollMgr *pm = new rrr::PollMgr();
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
			Log_info("current size of proxies is: %d", proxies_.size());
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
				std::vector<std::string> testserverAddr_ = serverAddr_;
				testserverAddr_.push_back(server_->addr_.c_str());
				std::string testString("hello_from_first");
				rrr::i32 result;
				auto cmd = std::make_shared<TestCommand>();
				cmd->a = 1;
				cmd->b = 2;
				cmd->c = 3;
				// cmd->result = result;
				// auto sp_m = dynamic_pointer_cast<Marshallable>(cmd);
				MarshallDeputy md(dynamic_pointer_cast<Marshallable>(cmd));


				// auto cmd_state = std::make_shared<TestCommandState>();
				// auto state_sp_m = dynamic_pointer_cast<Marshallable>(std::make_shared<TestCommandState>());
				MarshallDeputy state_md(dynamic_pointer_cast<Marshallable>(std::make_shared<TestCommandState>()));
				Log_info("**** inside demoserviceimpl::hello; in %s, calling cRPC", server_->addr_.c_str());
				proxies_[serverAddr_[index+1]]->cRPC(md, testserverAddr_, state_md);
				Log_info("**** inside demoserviceimpl::hello; in %s, returned from calling cRPC", server_->addr_.c_str());
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

		// abstract the computation to a helper function
		*result = DemoServiceImpl::sumHelper(a, b, c);
		// *result = a + b + c;
		defer->reply();
		Log_info("returning demoserviceimpl::sum in %s", server_->addr_.c_str());
	}

	rrr::i32 DemoServiceImpl::sumHelper(const rrr::i32& a, const rrr::i32& b, const rrr::i32& c){
		return a+b+c;
	}

	void DemoServiceImpl::cRPC(const MarshallDeputy& cmd, const std::vector<std::string>& addrChain, const MarshallDeputy& state, rrr::DeferredReply* defer){
		Log_info("inside demoserviceimpl::cRPC in %s", server_->addr_.c_str());
		Log_info("inside demoserviceimpl::cRPC; cmd type is:%d", cmd.kind_);
		// defer->reply();

		if (cmd.kind_ == MarshallDeputy::CMD_TEST)
		{
			// call wrapped function; here sum

			// //check the cmd.kind_ to know which upper function to call
			auto c = dynamic_pointer_cast<TestCommand>(cmd.sp_data_);
			Log_info("inside demoserviceimpl::cRPC; cmd message is: %d", c->a);

			if (proxies_.size() == 0)
			{
				populateProxies();
			}
			for (auto el : addrChain)
			{
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

			auto it = std::find(addrChain.begin(), addrChain.end(), server_->addr_);
			if (it != addrChain.end())
			{
				int index = it - addrChain.begin();

				std::string testString("hello_from_first");
				rrr::i32 result;
				// auto cmd = std::make_shared<TestCommand>();
				// cmd->a = c->a + 10;
				// cmd->b = c->b + 10;
				// cmd->c = c->c + 10;

				Log_info("**** inside demoserviceimpl::cRPC; calling sum");
				// proxies_[serverAddr_[index]]->sum(c->a + 10, c->b + 10, c->c + 10, &result);

				// // delete below when you figure out how to call self proxy
				// int randomNum =
				result = DemoServiceImpl::sumHelper(c->a + index, c->b + index, c->c + index);

				auto st = dynamic_pointer_cast<TestCommandState>(state.sp_data_);
				st->results.push_back(result);

				Log_info("**** inside demoserviceimpl::cRPC; in %s, results state is following", server_->addr_.c_str());
				for (auto el : st->results)
				{
					Log_info("**** inside demoserviceimpl::cRPC; in %s, result is: %d", server_->addr_.c_str(), el);
				}
				// cmd->result = result;
				// Log_info("**** inside demoserviceimpl::cRPC; in %s, result is: %d", server_->addr_.c_str(), result);
				// auto sp_m = dynamic_pointer_cast<Marshallable>(cmd);
				// MarshallDeputy md(sp_m);
				if (index < addrChain.size() - 1)
				{
					Log_info("**** inside demoserviceimpl::cRPC, in %s, calling cRPC on server: %s", server_->addr_.c_str(), addrChain[index + 1].c_str());
					proxies_[addrChain[index + 1]]->cRPC(cmd, addrChain, state);
					Log_info("**** inside demoserviceimpl::cRPC, in %s, returning after calling cRPC", server_->addr_.c_str());
				}
			}
		}

		Log_info("demoserviceimpl::cRPC; checkpoint 3 in %s", server_->addr_.c_str());
		defer->reply();
		Log_info("returning demoserviceimpl::cRPC in %s", server_->addr_.c_str());
	}
}
