
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

void SampleCrpcServiceImpl::CrpcAppendEntries(const uint64_t& id, 
                        const int64_t& value1,
                        const int64_t& value2,
                        const std::vector<uint16_t>& addrChain, 
                        const std::vector<ResultAdd>& state, 
                        rrr::DeferredReply* defer){
// just create a appendEntriesCommand. no casting required
// TODO: make Result as a base class and let AppendEntriesResult inherit it
  // Log_info("inside CrpcAppendEntries; checkpoint 0 @ %d", gettid());
  // const AppendEntriesCommand ae_cmd = AppendEntriesCommand (slot, 
  //                                     ballot, 
  //                                     leaderCurrentTerm, 
  //                                     leaderPrevLogIndex, 
  //                                     leaderPrevLogTerm, 
  //                                     leaderCommitIndex, 
  //                                     dep_id, 
  //                                     cmd);

  verify(sched_ != nullptr);
  Log_info("*** inside SampleCrpcServiceImpl::CrpcAppendEntries; tid: %d", gettid());
  if (!hasPrinted) {
      Log_info("tid of non-leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }

  // Coroutine::CreateRun([&] () {
  //     sched_->OnCRPC2(id,
  //                   ae_cmd,
  //                   addrChain,
  //                   state); // #profile (crpc2) - 4.96%
  //     defer->reply();
  // });

  // Log_info("*** inside SampleCrpcServiceImpl::CrpcAppendEntries; cp 2 tid: %d", gettid());
  Coroutine::CreateRun([&] () {
      sched_->OnCRPC3(id,
                    value1,
                    value2,
                    addrChain,
                    state);
      // Log_info("*** inside SampleCrpcServiceImpl::CrpcAppendEntries; cp 3 tid: %d", gettid());
      defer->reply();
      // Log_info("*** inside SampleCrpcServiceImpl::CrpcAppendEntries; cp 4 tid: %d", gettid());
  });

  // Log_info("*** returning from SampleCrpcServiceImpl::CrpcAppendEntries; tid: %d", gettid());
}

} // namespace janus;
