#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../communicator.h"

namespace janus {

class TxData;
class AppendEntriesCommand;
class AppendEntriesResult;

class SampleCrpcCommo : public Communicator {

friend class SampleCrpcProxy;
 public:
	std::unordered_map<siteid_t, uint64_t> matchedIndex {};
	int index;
	
  SampleCrpcCommo() = delete;
  SampleCrpcCommo(PollMgr*);

  void cRPC(const parid_t par_id,
              const uint64_t& id,
              const MarshallDeputy& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const MarshallDeputy& state);
};

} // namespace janus
