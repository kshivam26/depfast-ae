
#include "service.h"
#include "server.h"
#include "frame.h"
#include <thread>
#include <gperftools/profiler.h>
#include <sched.h>
#include <pthread.h>

namespace janus {
thread_local bool hasPrinted2 = false;
int i = 1;      // #follower start
int g = 1;      // #followers inside same core
int c = 1;      // #core per follower
int s = 2;      // #core start
bool x = true;  // #core exclusion

SampleCrpcServiceImpl::SampleCrpcServiceImpl(TxLogServer *sched) : sched_((SampleCrpcServer*)sched) {
	struct timespec curr_time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &curr_time);
	srand(curr_time.tv_nsec);
}

void SampleCrpcServiceImpl::CrpcAdd(const uint64_t& id, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, rrr::DeferredReply* defer){
  verify(sched_ != nullptr);
  //Log_info("*** inside SampleCrpcServiceImpl::CrpcAdd; tid: %d", gettid());
  if (!hasPrinted2) {
    if (addrChain.size() > 1) {
      thread_local pid_t t = gettid();
      Log_info("tid of non-leader is %d", t);
        thread_local cpu_set_t cs;
        CPU_ZERO(&cs);
        for (int k = 0; k < c; k++)
          CPU_SET(s+k-((!x) ? ((i-1) / g) : 0 ), &cs);
        s += (i % g == 0) ? c : 0;
        i++;
        verify(sched_setaffinity(t, sizeof(cs), &cs) == 0);
    }
    hasPrinted2 = true;  // Update the static variable
  }

  // if (addrChain.size() == 1) {
  //   auto x = (SampleCrpcCommo *)(sched_->commo_);
  //   verify(x->cRPCEvents.find(id) != x->cRPCEvents.end());
  //   auto ev = x->cRPCEvents[id];
  //   x->cRPCEvents.erase(id);
  //   for (size_t i = 0; i < state.size(); ++i) {
  //     bool y = true;
  //     ev->FeedResponse(y, i);
  //   }
  //   return;
  // }

  // ResultAdd res;
  // Coroutine::CreateRun([&]() { sched_->OnAdd(value1, value2, &res.result, []() {}); });
  // std::vector<ResultAdd> st(state);
  // st.push_back(res);
  // vector<uint16_t> addrChainCopy(addrChain.begin() + 1, addrChain.end());
  // // parid_t par_id = sched_->frame_->site_info_->partition_id_;
  // ((SampleCrpcCommo *)(sched_->commo_))->CrpcAdd3(id, value1, value2, addrChainCopy, st);

  Coroutine::CreateRun([&] () {
    sched_->OnCRPC3(id, value1, value2, addrChain, state);
    defer->reply();
  });

  // Log_info("*** returning from SampleCrpcServiceImpl::CrpcAdd; tid: %d", gettid());
}

void SampleCrpcServiceImpl::BroadcastAdd(const int64_t& value1, const int64_t& value2, int64_t *result, rrr::DeferredReply* defer) {
  verify(sched_ != nullptr);
  if (!hasPrinted2) {
      thread_local pid_t t = gettid();
      Log_info("tid of non-leader is %d", t);
      thread_local cpu_set_t cs;
      CPU_ZERO(&cs);
      for (int k = 0; k < c; k++)
        CPU_SET(s+k-((!x) ? ((i-1) / g) : 0 ), &cs);
      s += (i % g == 0) ? c : 0;
      i++;
      verify(sched_setaffinity(t, sizeof(cs), &cs) == 0);
      hasPrinted2 = true;  // Update the static variable
  }

  Coroutine::CreateRun([&] () {
    sched_->OnAdd(value1,
                            value2,
                            result,
                            std::bind(&rrr::DeferredReply::reply, defer));  // #profile - 3.42%

  });

  // Log_info("==== returning from SampleCrpcServiceImpl::Add");
	
}

} // namespace janus;
