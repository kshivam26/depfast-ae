
#include "service.h"
#include "server.h"
#include <thread>
# include <gperftools/profiler.h>

namespace janus {
static bool hasPrinted = false;

SampleCrpcServiceImpl::SampleCrpcServiceImpl(TxLogServer *sched)
    : sched_((SampleCrpcServer*)sched) {
	struct timespec curr_time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &curr_time);
	srand(curr_time.tv_nsec);
}

void SampleCrpcServiceImpl::CrpcAdd(const uint64_t& id, 
                        const int64_t& value1,
                        const int64_t& value2,
                        const std::vector<uint16_t>& addrChain, 
                        const std::vector<ResultAdd>& state, 
                        rrr::DeferredReply* defer){

  verify(sched_ != nullptr);
  //Log_info("*** inside SampleCrpcServiceImpl::CrpcAdd; tid: %d", gettid());
  if (!hasPrinted) {
      //Log_info("tid of non-leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }

  // Log_info("*** inside SampleCrpcServiceImpl::CrpcAdd; cp 2 tid: %d", gettid());
  //Log_info("*** --- inside SampleCrpcServiceImpl::CrpcAdd, entering OnCRPC3");
  Coroutine::CreateRun([&] () {
      sched_->OnCRPC3(id,
                    value1,
                    value2,
                    addrChain,
                    state);
      // Log_info("*** inside SampleCrpcServiceImpl::CrpcAdd; cp 3 tid: %d", gettid());
      defer->reply();
      // Log_info("*** inside SampleCrpcServiceImpl::CrpcAdd; cp 4 tid: %d", gettid());
  });

  // Log_info("*** returning from SampleCrpcServiceImpl::CrpcAdd; tid: %d", gettid());
}

void SampleCrpcServiceImpl::BroadcastAdd(const int64_t& value1,
                                        const int64_t& value2,
                                        int64_t *result,
                                        rrr::DeferredReply* defer) {
  verify(sched_ != nullptr);
  if (!hasPrinted) {
      //Log_info("*** inside SampleCrpcServiceImpl::BroadcastAdd tid of non-leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
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
