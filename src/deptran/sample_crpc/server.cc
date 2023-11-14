

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

  void SampleCrpcServer::Setup() {
    Log_info("*** inside SampleCrpcServer::Setup;");
  }

  SampleCrpcServer::~SampleCrpcServer() {
  }

  void SampleCrpcServer::OnAdd(const int64_t value1, const int64_t value2, int64_t *resultSum, const function<void()> &cb) {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        // Log_info("==== inside void SampleCrpcServer::OnAdd");

        *resultSum = value1 + value2;
        cb();  // #profile(n_crpc) - 1.15%
        // Log_info("==== returning from void SampleCrpcServer::OnAdd");
  }

  void SampleCrpcServer::OnCRPC3(const uint64_t& id, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state){
  //   //Log_info("*** inside SampleCrpcServer::OnCRPC; cp 1 tid: %d", gettid());
    if (addrChain.size() == 1) {
        // Log_info("==== reached the final link in the chain");
        // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 2 @ %d", gettid());
        // // add a verify statement
        auto x = (SampleCrpcCommo *)(this->commo_);
        verify(x->cRPCEvents.find(id) != x->cRPCEvents.end()); // #profile - 1.40%
        // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 2 @ %d", gettid());
        auto ev = x->cRPCEvents[id];
        x->cRPCEvents.erase(id);

        // Log_info("==== inside demoserviceimpl::cRPC; results state is following");
        // auto st = dynamic_pointer_cast<AddCommandState>(state.sp_data_);   // #profile - 0.54%
        for (size_t i = 0; i < state.size(); ++i) {
          // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 3 @ %d", gettid());
          verify(state[i].result == 3);
          ev->FeedResponse(true, i);
        }
        // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 4 @ %d", gettid());
        // Log_info("==== returning from cRPC");
        return;
    }
  
    ResultAdd res;
    // auto r = Coroutine::CreateRun([&]() { this->OnAdd(value1, value2, &res.result, []() {}); }); // #profile - 2.88%
    this->OnAdd(value1, value2, &res.result, []() {}); // #profile - 2.88%
    std::vector<ResultAdd> st(state);
    st.push_back(res);

    vector<uint16_t> addrChainCopy(addrChain.begin() + 1, addrChain.end());
    // auto addrChainCopy = addrChain;
    // addrChainCopy.erase(addrChainCopy.begin());
    // Log_info("inside SampleCrpcServer::OnCRPC3; calling CrpcAdd3");
    // Log_info("*** inside SampleCrpcServer::OnCRPC; cp 2 tid: %d", gettid());
    // parid_t par_id = this->frame_->site_info_->partition_id_;
    ((SampleCrpcCommo *)(this->commo_))->CrpcAdd3(id, value1, value2, addrChainCopy, st);
    // Log_info("*** inside SampleCrpcServer::OnCRPC; cp 3 tid: %d", gettid());
  }

} // namespace janus
