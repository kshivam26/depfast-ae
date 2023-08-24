
#include "../__dep__.h"
#include "../constants.h"
#include "coordinator.h"
#include "commo.h"
// #include <chrono>
#include "server.h"

namespace janus {

CoordinatorSampleCrpc::CoordinatorSampleCrpc(uint32_t coo_id,
                                             int32_t benchmark,
                                             ClientControlServiceImpl* ccsi,
                                             uint32_t thread_id)
    : Coordinator(coo_id, benchmark, ccsi, thread_id) {
    Log_info("*** inside CoordinatorSampleCrpc::CoordinatorSampleCrpc");
}

void CoordinatorSampleCrpc::Submit(shared_ptr<Marshallable>& cmd,
                                   const function<void()>& func,
                                   const function<void()>& exe_callback) {
  Log_info("*** inside void CoordinatorSampleCrpc::Submit");
  verify(cmd_ == nullptr);
  cmd_ = cmd;
  verify(cmd_->kind_ != MarshallDeputy::UNKNOWN);
  AppendEntries();
  Log_info("*** returning from void CoordinatorSampleCrpc::Submit");
}

void CoordinatorSampleCrpc::AppendEntries() {

    Log_info("*** inside void CoordinatorSampleCrpc::AppendEntries;");
    // Log_info("*** inside void CoordinatorSampleCrpc::AppendEntries; count: %ld; tid is: %d", count, gettid());

    int64_t value1 = 2;
    int64_t value2 = 3;
    commo()->crpc_add(par_id_, value1, value2, cmd_);

    // Log_info("*** returning from void CoordinatorSampleCrpc::AppendEntries");
}

} // namespace janus