#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../scheduler.h"
#include "../classic/tpc_command.h"
#include "append_entries_command.h"

namespace janus {
class Command;
class CmdData;

class SampleCrpcServer : public TxLogServer {
 public:
  SampleCrpcServer(Frame *frame) ;
  ~SampleCrpcServer() ;

  void OnCRPC3(const uint64_t& id,
              const MarshallDeputy& cmd,
              const std::vector<uint16_t>& addrChain, 
              const std::vector<AppendEntriesAdd>& state);

  void removeCmd(slotid_t slot);
};
} // namespace janus