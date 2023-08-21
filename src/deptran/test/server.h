#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../scheduler.h"
#include "../classic/tpc_command.h"

namespace janus {

class TestServer : public TxLogServer {
  private:
    void Setup();
  public:
    TestServer(Frame *frame) ;
    ~TestServer() ;
    void OnCRPC(const uint64_t& id,
                const MarshallDeputy& cmd, 
                const std::vector<uint16_t>& addrChain, 
                const MarshallDeputy& state);
};

} // namespace janus
