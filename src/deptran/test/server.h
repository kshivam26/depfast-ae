#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../scheduler.h"
#include "../classic/tpc_command.h"

namespace janus {

class TestServer : public TxLogServer {
  private:
    void Setup();
    bool stop_ = false ;
  public:
    uint64_t lastLogIndex = 0;
    uint64_t commitIndex = 0;
    uint64_t toyCounter = 0;
    TestServer(Frame *frame) ;
    ~TestServer() ;
    void cRPCSRV(const uint64_t& id, const std::vector<uint16_t>& addrChain, const MarshallDeputy& cmd);
};

} // namespace janus
