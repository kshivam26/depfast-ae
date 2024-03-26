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
    TestServer(Frame *frame) ;
    ~TestServer() ;
    void OnAdd(const int64_t& value, int64_t *result, const function<void()> &cb);
    void cRPCSRV(const uint64_t& id, const int64_t& value, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state);
    // void cRPCSRV_B(const MarshallDeputy& cmd, uint64_t *AcceptOK, const function<void()> &cb);
};

} // namespace janus
