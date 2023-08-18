#pragma once

#include "__dep__.h"
#include "constants.h"
#include "../rcc/graph.h"
#include "../rcc/graph_marshaler.h"
#include "../command.h"
#include "deptran/procedure.h"
#include "../command_marshaler.h"
#include "../rcc_rpc.h"

class SimpleCommand;
namespace janus {

class TxLogServer;
class SampleCrpcServer;
class SampleCrpcServiceImpl : public SampleCrpcService {
 public:
  SampleCrpcServer* sched_;
  SampleCrpcServiceImpl(TxLogServer* sched);
  void cRPC(const uint64_t& id,
            const MarshallDeputy& cmd, 
            const std::vector<uint16_t>& addrChain, 
            const MarshallDeputy& state, 
            rrr::DeferredReply* defer) override;

};

} // namespace janus
