#include "server.h"
#include "frame.h"
#include "coordinator.h"
#include "../classic/tpc_command.h"
#include <gperftools/profiler.h>

namespace janus {

TestServer::TestServer(Frame * frame) {
  frame_ = frame ;
  stop_ = false ;
}

void TestServer::Setup() {}

TestServer::~TestServer() {
	stop_ = true;
}

void TestServer::OnAdd(const int64_t& value, int64_t *result, const function<void()> &cb) {
  std::lock_guard<std::recursive_mutex> lock(mtx_);
  *result = value + value;
  cb();
}

void TestServer::cRPCSRV(const uint64_t& id, const int64_t& value, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state) {
  // Log_info("==== inside void TestServer::cRPCSRV; counter: %ld; tid is: %d", toyCounter, gettid());
    if (addrChain.size() == 1) {
        // Log_info("==== reached the final link in the chain");
        // // Log_info("inside TestServer::cRPCSRV; checkpoint 1 @ %d", gettid());
        // // add a verify statement
        auto x = (TestCommo *)(this->commo_);
        verify(x->cRPCEvents.find(id) != x->cRPCEvents.end()); // #profile - 1.40%
        // // Log_info("inside TestServer::cRPCSRV; checkpoint 2 @ %d", gettid());
        auto ev = x->cRPCEvents[id];
        x->cRPCEvents.erase(id);

        // // Log_info("==== inside demoserviceimpl::cRPC; results state is following");
        // auto st = dynamic_pointer_cast<AppendEntriesCommandState>(state.sp_data_);   // #profile - 0.54%
        // for (auto el : state) {
          // // Log_info("inside TestServer::cRPCSRV; checkpoint 3 @ %d", gettid());
          // bool y = ((el.followerAppendOK == 1) && (this->IsLeader()) && (currentTerm == el.followerCurrentTerm));
          // ev->FeedResponse(y, el.followerLastLogIndex);
          for (auto el : state) ev->FeedResponse(true, 1);
        // }
        // // Log_info("inside TestServer::cRPCSRV; checkpoint 4 @ %d", gettid());
        // Log_info("==== returning from cRPC; tid is: %d", gettid());
        return;
    }

  std::vector<ResultAdd> st(state);
  ResultAdd res;
  // auto r = Coroutine::CreateRun([&](){ this->OnAdd(value, &res.result, []() {}); });
  this->OnAdd(value, &res.result, []() {});
  st.push_back(res);

  vector<uint16_t> addrChainCopy(addrChain.begin() + 1, addrChain.end());
  parid_t par_id = this->frame_->site_info_->partition_id_;
  ((TestCommo *)(this->commo_))->cRPC2(id, value, addrChainCopy, st);
}

// void TestServer::cRPCSRV_B(const MarshallDeputy& cmd, uint64_t *AcceptOK, const function<void()> &cb) {
//   // Log_info("@@@ Test CP 19A: TestServer::cRPCSRV_B");
//   std::lock_guard<std::recursive_mutex> lock(mtx_);

//   // // Log_info("==== inside void TestServer::cRPCSRV_B; counter: %ld; tid is: %d", toyCounter, gettid());
//   *AcceptOK = 1;
//   cb();
//   // Log_info("==== returning from TestServer::cRPCSRV_B; counter: %ld; tid is: %d", toyCounter, gettid());
// }

}
