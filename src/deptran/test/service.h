#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../rcc/graph.h"
#include "../rcc/graph_marshaler.h"
#include "../command.h"
#include "../procedure.h"
#include "../command_marshaler.h"
#include "../rcc_rpc.h"

class SimpleCommand;
namespace janus {

class TxLogServer;
class TestServer;
class TestServiceImpl : public TestService {
  public:
    TestServer* sched_;
    TestServiceImpl(TxLogServer* sched);
    void cRPCSVC(const uint64_t& id, const int64_t& value, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, rrr::DeferredReply* defer) override;
    void cRPCSVC_B(const int64_t& value, int64_t *result, rrr::DeferredReply* defer) override;
};

} // namespace janus
