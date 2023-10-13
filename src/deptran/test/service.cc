#include "service.h"
#include "server.h"
#include <thread>
#include <gperftools/profiler.h>

namespace janus {
thread_local bool tidPrinted = false;

TestServiceImpl::TestServiceImpl(TxLogServer *sched)
    : sched_((TestServer *)sched) {
}

void TestServiceImpl::cRPCSVC(const uint64_t& id, const int64_t& value, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, rrr::DeferredReply* defer) {
  // Log_info("==== inside void TestServiceImpl::cRPCSVC");
  // just create a appendEntriesCommand. no casting required
  // TODO: make Result as a base class and let ResultAdd inherit it
  // // Log_info("inside cRPCSVC; checkpoint 0 @ %d", gettid());
  // const AppendEntriesCommand ae_cmd = AppendEntriesCommand (slot,
  //                                     ballot,
  //                                     leaderCurrentTerm,
  //                                     leaderPrevLogIndex,
  //                                     leaderPrevLogTerm,
  //                                     leaderCommitIndex,
  //                                     dep_id,
  //                                     cmd);

  verify(sched_ != nullptr);
  // // Log_info("*** inside TestServiceImpl::cRPCSVC; tid: %d", gettid());
  if (!tidPrinted) {
    Log_info("tid of non-leader is %d", gettid());
    tidPrinted = true;  // Update the static variable
  }

  // Coroutine::CreateRun([&] () {
  //     sched_->OnCRPC2(id,
  //                   ae_cmd,
  //                   addrChain,
  //                   state); // #profile (crpc2) - 4.96%
  //     defer->reply();
  // });

  // // Log_info("*** inside TestServiceImpl::cRPCSVC; cp 2 tid: %d", gettid());
  Coroutine::CreateRun([&] () {
    sched_->cRPCSRV(id, value, addrChain, state);
    // // Log_info("*** inside TestServiceImpl::cRPCSVC; cp 3 tid: %d", gettid());
    defer->reply();
    // // Log_info("*** inside TestServiceImpl::cRPCSVC; cp 4 tid: %d", gettid());
  });
  // Log_info("*** returning from TestServiceImpl::cRPCSVC; tid: %d", gettid());
}

void TestServiceImpl::cRPCSVC_B(const int64_t& value, int64_t *result, rrr::DeferredReply* defer) {
  // // Log_info("@@@ Test CP 15A: TestServiceImpl::cRPCSVC_B");
  // Log_info("==== inside void TestServiceImpl::cRPCSVC_B");
  verify(sched_ != nullptr);
  // // Log_info("*** inside TestServiceImpl::cRPCSVC_B; tid: %d", gettid());
  if (!tidPrinted) {
    Log_info("tid of non-leader is %d", gettid());
    tidPrinted = true;  // Update the static variable
  }

  // // Log_info("*** inside TestServiceImpl::_B; cp 2 tid: %d", gettid());
  Coroutine::CreateRun([&] () {
    sched_->OnAdd(value, result, std::bind(&rrr::DeferredReply::reply, defer));
    // // Log_info("*** inside TestServiceImpl::cRPCSVC_B; cp 3 tid: %d", gettid());
    // defer->reply();
    // // Log_info("*** inside TestServiceImpl::cRPCSVC_B; cp 4 tid: %d", gettid());
  });
  // Log_info("*** returning from TestServiceImpl::cRPCSVC_B; tid: %d", gettid());
}

}
