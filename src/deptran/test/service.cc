#include "service.h"
#include "server.h"

namespace janus {
static bool hasPrinted = false;

TestServiceImpl::TestServiceImpl(TxLogServer *sched)
    : sched_((TestServer *)sched) {
}

void TestServiceImpl::cRPC(const uint64_t& id,
                           const MarshallDeputy& cmd, 
                           const std::vector<uint16_t>& addrChain, 
                           const MarshallDeputy& state, 
                           rrr::DeferredReply* defer) {
  Log_info("==== inside void TestServiceImpl::cRPC");

  verify(sched_ != nullptr);  // #profile - 0.9%

  // static bool hasPrinted = false;  // Static variable to track if it has printed

  if (!hasPrinted) {
      Log_info("tid of non-leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }
  // ProfilerStart("P1.prof");
  Coroutine::CreateRun([&] () {
      // defer->reply();
      // Log_info("==== calling first onCRPC");
        
      sched_->OnCRPC(id,
                    cmd,
                    addrChain,
                    state); // #profile - 4.28%; decide has like 7%
      // Log_info("==== now will do a defer->reply");
      defer->reply();
      });  // #profile - 1.17%
  // ProfilerStop();
}

}
