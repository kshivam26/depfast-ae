#include "demo.h"
#include <map>
#include "../src/rrr/rpc/client.hpp"
#include "../src/rrr/reactor/reactor.h"
#include "commo.h"
#include<vector>
namespace janus{
 

class DemoServiceImpl: public DemoService {
	public:
	rrr::Server* server_;
	std::vector<std::string> serverAddr_;
	std::map<std::string, janus::DemoProxy *> proxies_;
	// Communicator* commo_ = nullptr;
	DemoServiceImpl(rrr::Server* s, std::vector<std::string> serverAddr);
	// janus::Communicator* findOrCreateCommo();
	void OnHello();
	void populateProxies();
	rrr::i32 sumHelper(const rrr::i32& a, const rrr::i32& b, const rrr::i32& c);
	void hello(const std::string& hi, std::string* reply, rrr::DeferredReply* defer);
	void sum(const rrr::i32& a, const rrr::i32& b, const rrr::i32& c, rrr::i32* result, rrr::DeferredReply* defer);
	void cRPC(const MarshallDeputy& cmd, const std::vector<std::string>& addrChain, const MarshallDeputy& state, rrr::DeferredReply* defer);
};
}
		
			
