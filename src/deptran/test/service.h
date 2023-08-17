#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../rcc/graph.h"
#include "../rcc/graph_marshaler.h"
#include "../command.h"
#include "../procedure.h"
#include "../command_marshaler.h"
#include "../rcc_rpc.h"

namespace janus {

class TxLogServer;
class TestServer;

class TestServiceImpl : public TestService {
  TestServer* sched_;
 public:
  TestServiceImpl(TxLogServer *sched);
  void cRPC(const uint64_t& id,
            const MarshallDeputy& cmd, 
            const std::vector<uint16_t>& addrChain, 
            const MarshallDeputy& state, 
            rrr::DeferredReply* defer) override;
};

} // namespace janus
