#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../scheduler.h"
#include "../classic/tpc_command.h"
#include "../rcc/graph.h"
#include "../rcc/graph_marshaler.h"
#include "../command.h"
#include "../procedure.h"
#include "../command_marshaler.h"
#include "../rcc_rpc.h"

namespace janus {

class TestServer : public TxLogServer {
  private:
    void Setup();
    bool stop_ = false ;
  public:
    uint64_t toyCounter = 0;
    TestServer(Frame *frame) ;
    ~TestServer() ;
    void cRPCSRV(const uint64_t& id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, const std::vector<AppendEntriesResult>& state);
};

} // namespace janus
