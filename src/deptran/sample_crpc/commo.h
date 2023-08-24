#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../communicator.h"

namespace janus {

class TxData;
class ResultCommand;

class SampleCrpcCommo : public Communicator {

friend class SampleCrpcProxy;
 public:
	SampleCrpcCommo() = delete;
  SampleCrpcCommo(PollMgr*);

  void crpc_add(parid_t par_id,
              const int64_t& value1,
              const int64_t& value2,
              shared_ptr<Marshallable> cmd);

  void CrpcAppendEntries(const parid_t par_id,
              const uint64_t& id,
              const int64_t& value1,
              const int64_t& value2,
              const std::vector<uint16_t>& addrChain, 
              const std::vector<ResultAdd>& state);
};

} // namespace janus
