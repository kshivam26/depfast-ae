#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../scheduler.h"
#include "../classic/tpc_command.h"

namespace janus {
class Command;
class CmdData;
class ResultAdd;

class SampleCrpcServer : public TxLogServer {
 public:
  SampleCrpcServer(Frame *frame) ;
  ~SampleCrpcServer() ;

  void OnAppendEntries(const int64_t value1,
                       const int64_t value2,
                       int64_t *resultSum,
                       const function<void()> &cb);

  void OnCRPC3(const uint64_t& id,
              const int64_t& value1,
              const int64_t& value2,
              const std::vector<uint16_t>& addrChain, 
              const std::vector<ResultAdd>& state);

  void removeCmd(slotid_t slot);
 private:
  void Setup();
};
} // namespace janus
