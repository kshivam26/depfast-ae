#include "service.h"
#include "server.h"

namespace janus {

TestServer::TestServer(Frame * frame) {
  frame_ = frame ;
}

TestServer::~TestServer() {}

void TestServer::OnCRPC(const uint64_t& id,
                            const MarshallDeputy& cmd, 
                            const std::vector<uint16_t>& addrChain, 
                            const MarshallDeputy& state){
  Log_info("==== inside void TestServer::OnCRPC");
}

}
