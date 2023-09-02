#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../communicator.h"
#include "server.h"

namespace janus {

class TxData;

class ChainQuorumEvent: public QuorumEvent {
  public:
    uint64_t minIndex;
    using QuorumEvent::QuorumEvent;
    void FeedResponse(bool appendOK, uint64_t index, std::string ip_addr = "") {
      // Log_info("==== inside ChainQuorumEvent:FeedResponse");
      if (appendOK) {
        // Log_info("==== inside ChainQuorumEvent:FeedRespons; checkpoint 0");
          if ((n_voted_yes_ == 0) && (n_voted_no_ == 0)){
            // Log_info("==== inside ChainQuorumEvent:FeedRespons; checkpoint 00");
              minIndex = index;
          } else{
            // Log_info("==== inside ChainQuorumEvent:FeedRespons; checkpoint 01");
            minIndex = std::min(minIndex, index);
          }
          VoteYes();
      } else {
        Log_info("==== inside ChainQuorumEvent:FeedRespons; checkpoint 1");
        VoteNo();
      }
      /*Log_debug("fpga-raft comm accept event, "
        "yes vote: %d, no vote: %d, min index: %d",
        n_voted_yes_, n_voted_no_, minIndex);*/
      // Log_info("==== inside ChainQuorumEvent:FeedRespons; checkpoint final");
    }
};

class TestCommo : public Communicator {
friend class TestProxy;
  public:
    std::unordered_map<uint64_t, shared_ptr<ChainQuorumEvent>> cRPCEvents {};
    int index;

    TestCommo() = delete;
    TestCommo(PollMgr*);
    shared_ptr<ChainQuorumEvent> cRPC(parid_t par_id, siteid_t leader_site_id, shared_ptr<Marshallable> cmd);
    void cRPC2(const uint64_t& id, const std::vector<uint16_t>& addrChain, const MarshallDeputy& cmd);
};

}
