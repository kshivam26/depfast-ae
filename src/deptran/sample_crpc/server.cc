

#include "server.h"
// #include "paxos_worker.h"
#include "frame.h"
#include "coordinator.h"
#include "../classic/tpc_command.h"
# include <gperftools/profiler.h>

namespace janus {

SampleCrpcServer::SampleCrpcServer(Frame * frame) {
  frame_ = frame ;
}

SampleCrpcServer::~SampleCrpcServer() {
}

  void SampleCrpcServer::OnCRPC(const uint64_t& id,
              const MarshallDeputy& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const MarshallDeputy& state){
    Log_info("==== inside void SampleCrpcServer::OnCRPC");

    // switch (cmd.kind_){
    //   case MarshallDeputy::CMD_RAFT_APPEND_ENTRIES:
    //     Log_info("==== inside switch->CMD_RAFT_APPEND_ENTRIES");

    //     if (addrChain.size() == 1){
    //       Log_info("==== reached the final link in the chain");

    //       // // add a verify statement
    //       auto x = (SampleCrpcCommo *)(this->commo_);
    //       verify(x->cRPCEvents.find(id) != x->cRPCEvents.end()); // #profile - 1.40%
    //       auto ev = x->cRPCEvents[id];
    //       x->cRPCEvents.erase(id);

    //       Log_info("==== inside demoserviceimpl::cRPC; results state is following");
    //       auto st = dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_);   // #profile - 0.54%
    //       for (auto el : st->results)
    //       {
    //         bool y = ((el.followerAppendOK == 1) && (this->IsLeader()) && (currentTerm == el.followerCurrentTerm));
    //         ev->FeedResponse(y, el.followerLastLogIndex);
    //       }
    //       Log_info("==== returning from cRPC");
    //       return;
    //     }

    //     // Log_info("calling dynamic_pointer_cast<AppendEntriesCommand>(state.sp_data_)");
    //     auto c = dynamic_pointer_cast<AppendEntriesCommand>(cmd.sp_data_);
    //     // Log_info("return dynamic_pointer_cast<AppendEntriesCommand>(state.sp_data_)");
    //     AppendEntriesResult res;
    //     auto r = Coroutine::CreateRun([&]()   
    //                                   { this->OnAppendEntries(c->slot,
    //                                                           c->ballot,
    //                                                           c->leaderCurrentTerm,
    //                                                           c->leaderPrevLogIndex,
    //                                                           c->leaderPrevLogTerm,
    //                                                           c->leaderCommitIndex,
    //                                                           c->dep_id,
    //                                                           const_cast<MarshallDeputy &>(c->md_cmd).sp_data_,
    //                                                           &res.followerAppendOK,
    //                                                           &res.followerCurrentTerm,
    //                                                           &res.followerLastLogIndex,
    //                                                           [](){}); });  // #profile - 2.88%
    //     // Log_info("calling dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_)");
    //     auto st = dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_);  // #profile - 1.23%  ==> dont think can do anything about it
    //     // Log_info("returned dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_)");
    //     st->results.push_back(res);

    //     auto addrChainCopy = addrChain;
    //     addrChainCopy.erase(addrChainCopy.begin());
        
    //     parid_t par_id = this->frame_->site_info_->partition_id_;
    //     ((SampleCrpcCommo *)(this->commo_))->cRPC(par_id, id, cmd, addrChainCopy, state);   // #profile - 1.77%
    //     Log_info("==== returning from void SampleCrpcServer::OnCRPC");
    // }
  }

} // namespace janus
