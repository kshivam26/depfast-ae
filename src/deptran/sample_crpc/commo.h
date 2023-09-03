#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../communicator.h"

namespace janus {

class TxData;
class ResultCommand;

class SampleCrpcAppendQuorumEvent: public QuorumEvent {
 public:
    uint64_t minIndex;
    using QuorumEvent::QuorumEvent;
    void FeedResponse(bool appendOK, uint64_t index, std::string ip_addr = "") {
        // Log_info("==== inside SampleCrpcAppendQuorumEvent:FeedResponse");
        if (appendOK) {
          // Log_info("==== inside SampleCrpcAppendQuorumEvent:FeedRespons; checkpoint 0");
            if ((n_voted_yes_ == 0) && (n_voted_no_ == 0)){
                // Log_info("==== inside SampleCrpcAppendQuorumEvent:FeedRespons; checkpoint 00");
                minIndex = index;
            }
            else{
                // Log_info("==== inside SampleCrpcAppendQuorumEvent:FeedRespons; checkpoint 01");
                minIndex = std::min(minIndex, index);
            }
            VoteYes();
        } else {
            Log_info("==== inside SampleCrpcAppendQuorumEvent:FeedRespons; checkpoint 1");
            VoteNo();
        }
        /*Log_debug("fpga-raft comm accept event, "
                  "yes vote: %d, no vote: %d, min index: %d",
                  n_voted_yes_, n_voted_no_, minIndex);*/
        // Log_info("==== inside SampleCrpcAppendQuorumEvent:FeedRespons; checkpoint final");
    }
};

class SampleCrpcCommo : public Communicator {

friend class SampleCrpcProxy;
 public:
  std::unordered_map<uint64_t, shared_ptr<SampleCrpcAppendQuorumEvent>> cRPCEvents {};

	SampleCrpcCommo() = delete;
  SampleCrpcCommo(PollMgr*);

  shared_ptr<SampleCrpcAppendQuorumEvent> crpc_add(parid_t par_id,
              siteid_t leader_site_id,
              const int64_t& value1,
              const int64_t& value2,
              shared_ptr<Marshallable> cmd);

  void CrpcAppendEntries3(const parid_t par_id,
              const uint64_t& id,
              const int64_t& value1,
              const int64_t& value2,
              const std::vector<uint16_t>& addrChain, 
              const std::vector<ResultAdd>& state);
};

} // namespace janus
