

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

  void SampleCrpcServer::OnCRPC3(const uint64_t& id,
              const MarshallDeputy& cmd,
              const std::vector<uint16_t>& addrChain, 
              const std::vector<AppendEntriesAdd>& state){
    Log_info("*** inside SampleCrpcServer::OnCRPC; cp 1 tid: %d", gettid());
    // if (addrChain.size() == 1)
    // {
    //     // Log_info("==== reached the final link in the chain");
    //     // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 1 @ %d", gettid());
    //     // // add a verify statement
    //     auto x = (SampleCrpcCommo *)(this->commo_);
    //     verify(x->cRPCEvents.find(id) != x->cRPCEvents.end()); // #profile - 1.40%
    //     // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 2 @ %d", gettid());
    //     auto ev = x->cRPCEvents[id];
    //     x->cRPCEvents.erase(id);

    //     // Log_info("==== inside demoserviceimpl::cRPC; results state is following");
    //     // auto st = dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_);   // #profile - 0.54%
    //     for (auto el : state)
    //     {
    //       // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 3 @ %d", gettid());
    //       bool y = ((el.followerAppendOK == 1) && (this->IsLeader()) && (currentTerm == el.followerCurrentTerm));
    //       ev->FeedResponse(y, el.followerLastLogIndex);
    //     }
    //     // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 4 @ %d", gettid());
    //     // Log_info("==== returning from cRPC");
    //     return;
    // }

    // // Log_info("calling dynamic_pointer_cast<AppendEntriesCommand>(state.sp_data_)");
    // // auto c = dynamic_pointer_cast<AppendEntriesCommand>(cmd.sp_data_);
    // // Log_info("return dynamic_pointer_cast<AppendEntriesCommand>(state.sp_data_)");
    // AppendEntriesResult res;
    // auto r = Coroutine::CreateRun([&]()
    //                               { this->OnAppendEntries(slot_id,
    //                                                       ballot,
    //                                                       leaderCurrentTerm,
    //                                                       leaderPrevLogIndex,
    //                                                       leaderPrevLogTerm,
    //                                                       leaderCommitIndex,
    //                                                       dep_id,
    //                                                       const_cast<MarshallDeputy &>(cmd).sp_data_,
    //                                                       &res.followerAppendOK,
    //                                                       &res.followerCurrentTerm,
    //                                                       &res.followerLastLogIndex,
    //                                                       []() {}); }); // #profile - 2.88%
    // // Log_info("###################cp1");
    // // this->OnAppendEntries(slot_id,
    // //                               ballot,
    // //                               leaderCurrentTerm,
    // //                               leaderPrevLogIndex,
    // //                               leaderPrevLogTerm,
    // //                               leaderCommitIndex,
    // //                               dep_id,
    // //                               const_cast<MarshallDeputy &>(cmd).sp_data_,
    // //                               &res.followerAppendOK,
    // //                               &res.followerCurrentTerm,
    // //                               &res.followerLastLogIndex,
    // //                               []() {});
    // // Log_info("###################cp2");
    // // Log_info("calling dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_)");
    // std::vector<AppendEntriesResult> st(state);
    // // auto st = dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_);  // #profile - 1.23%  ==> dont think can do anything about it
    // // Log_info("returned dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_)");
    // st.push_back(res);

    // vector<uint16_t> addrChainCopy(addrChain.begin() + 1, addrChain.end());
    // // auto addrChainCopy = addrChain;
    // // addrChainCopy.erase(addrChainCopy.begin());
    // // Log_info("inside SampleCrpcServer::OnCRPC3; calling CrpcAppendEntries3");
    // // Log_info("*** inside SampleCrpcServer::OnCRPC; cp 2 tid: %d", gettid());
    // parid_t par_id = this->frame_->site_info_->partition_id_;
    // ((SampleCrpcCommo *)(this->commo_))->CrpcAppendEntries3(par_id, id, 
    //                                                     slot_id,
    //                                                       ballot,
    //                                                       leaderCurrentTerm,
    //                                                       leaderPrevLogIndex,
    //                                                       leaderPrevLogTerm,
    //                                                       leaderCommitIndex,
    //                                                       dep_id,
    //                                                       cmd, addrChainCopy, st); // #profile (crpc2) - 4.02%%
    //                                                                                           // Log_info("==== returning from void SampleCrpcServer::OnCRPC");
    // // Log_info("*** inside SampleCrpcServer::OnCRPC; cp 3 tid: %d", gettid());
  }

} // namespace janus
