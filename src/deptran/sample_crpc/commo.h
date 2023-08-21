#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../communicator.h"

namespace janus {

class TxData;
class AppendEntriesCommandToyCrpc;
class AppendEntriesAdd;

class SampleCrpcCommo : public Communicator {

friend class SampleCrpcProxy;
 public:
	std::unordered_map<siteid_t, uint64_t> matchedIndex {};
	int index;
	
  SampleCrpcCommo() = delete;
  SampleCrpcCommo(PollMgr*);

  void CrpcAppendEntries(const parid_t par_id,
              const uint64_t& id,
              const AppendEntriesCommandToyCrpc& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const std::vector<AppendEntriesAdd>& state);
};

} // namespace janus
