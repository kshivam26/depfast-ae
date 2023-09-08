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
  void CrpcAdd(const uint64_t& id,
                          const int64_t& value1,
                          const int64_t& value2,
                          const std::vector<uint16_t>& addrChain, 
                          const std::vector<ResultAdd>& state, 
                          rrr::DeferredReply* defer) override;
                          
  void BroadcastAdd(const int64_t& value1,
                          const int64_t& value2,
                          int64_t *result,
                          rrr::DeferredReply* defer) override;

};

} // namespace janus
