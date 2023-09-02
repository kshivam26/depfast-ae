#include "../__dep__.h"
#include "../constants.h"
#include "coordinator.h"
#include "commo.h"
#include "server.h"

namespace janus {

CoordinatorTest::CoordinatorTest(uint32_t coo_id,
                                 int32_t benchmark,
                                 ClientControlServiceImpl *ccsi,
                                 uint32_t thread_id) : Coordinator(coo_id, benchmark, ccsi, thread_id) {
  Log_info("@@@ Test CP 7: CoordinatorTest::CoordinatorTest");

  Log_info("*** inside CoordinatorTest");
}

CoordinatorTest::~CoordinatorTest() {
  Log_info("@@@ Test CP 8: CoordinatorTest::~CoordinatorTest");
  Log_info("Test coord %d destroyed", (int)coo_id_);
}

void CoordinatorTest::Submit(shared_ptr<Marshallable>& cmd,
                                   const function<void()>& func,
                                   const function<void()>& exe_callback) {
  Log_info("@@@ Test CP 9: CoordinatorTest::Submit");
  Log_info("*** inside void CoordinatorTest::Submit");
  /* if (!IsLeader()) {
    //Log_fatal("i am not the leader; site %d; locale %d",
    //          frame_->site_info_->id, loc_id_);
    Forward(cmd, func, exe_callback) ;
    return ;
  } */
  std::lock_guard<std::recursive_mutex> lock(mtx_);
  verify(!in_submission);
  verify(cmd_ == nullptr);
  // verify(cmd.self_cmd_ != nullptr);
  in_submission = true;
  cmd_ = cmd;
  verify(cmd_->kind_ != MarshallDeputy::UNKNOWN);
  commit_callback_ = func;
  StartChain();
  Log_info("*** returning from void CoordinatorTest::Submit");
}

void CoordinatorTest::StartChain() {
  Log_info("@@@ Test CP 10: CoordinatorTest::StartChain");

  Log_info("*** inside void CoordinatorTest::StartChain");

  std::lock_guard<std::recursive_mutex> lock(mtx_);
  verify(!in_start_chain);
  in_start_chain = true;

  // uint64_t prevLogIndex = this->sch_->lastLogIndex;

  shared_ptr<ChainQuorumEvent> sp_quorum = nullptr;
  // sp_quorum = commo()->cRPC(par_id_, this->sch_->site_id_, prevLogIndex, dep_id_, cmd_);
  sp_quorum = commo()->cRPC(par_id_, this->sch_->site_id_, cmd_);

  // struct timespec start_, end_;
  // clock_gettime(CLOCK_MONOTONIC, &start_);
  // Log_info("=== waiting for quorum");
  sp_quorum->Wait();
  // Log_info("*** quorum reached");
  // struct timespec end_;
  // clock_gettime(CLOCK_MONOTONIC, &end_);

  // quorum_events_.push_back(sp_quorum);
  // Log_info("*** time of sp_quorum->Wait(): %ld", (end_.tv_sec-start_.tv_sec)* 1000000L + (end_.tv_nsec-start_.tv_nsec)/1000L);
  // slow_ = sp_quorum->IsSlow();  // #profile - 2.13%

  // long leader_time;
  // std::vector<long> follower_times {};

  // int total_ob = 0;
  // int avg_ob = 0;
  // Log_info("begin_index: %d", commo()->begin_index);
  // if (commo()->begin_index >= 1000) {
  //   if (commo()->ob_index < 100) {
  //     commo()->outbounds[commo()->ob_index] = commo()->outbound;
  //     commo()->ob_index++;
  //   } else {
  //     for (int i = 0; i < 99; i++) {
  //       commo()->outbounds[i] = commo()->outbounds[i+1];
	// total_ob += commo()->outbounds[i];
  //     }
  //     commo()->outbounds[99] = commo()->outbound;
  //     total_ob += commo()->outbounds[99];
  //   }
  //   commo()->begin_index = 0;
  // } else {
  //   commo()->begin_index++;
  // }
  // avg_ob = total_ob/100;

  // for (auto it = commo()->rpc_clients_.begin(); it != commo()->rpc_clients_.end(); it++) {
  //   if (avg_ob > 0 && it->second->time_ > 0) Log_info("time for %d is: %d", it->first, it->second->time_/avg_ob);
  //   if (it->first != loc_id_) follower_times.push_back(it->second->time_);
  // }
  // if (avg_ob > 0 && !slow_) {
  //   Log_debug("number of rpcs: %d", avg_ob);
  //   Log_debug("%d and %d", follower_times[0]/avg_ob, follower_times[1]/avg_ob);
  //   slow_ = follower_times[0]/avg_ob > 80000 && follower_times[1]/avg_ob > 80000;
  // }

  // Log_info("slow?: %d", slow_);
  // if (sp_quorum->Yes()) {
  //   minIndex = sp_quorum->minIndex;
  //   Log_info("%d vs %d", minIndex, this->sch_->commitIndex);
  //   verify(minIndex >= this->sch_->commitIndex) ;
  //   committed_ = true;
  //   Log_debug("fpga-raft append commited loc:%d minindex:%d", loc_id_, minIndex ) ;
  // } else if (sp_quorum->No()) {
  //     verify(0);
      // TODO should become a follower if the term is smaller
      // if(!IsLeader())
      /* {
        Forward(cmd_,commit_callback_) ;
        return ;
      } */
  // } else {
  //     verify(0);
  // }

  Log_info("*** returning from void CoordinatorTest::StartChain");
}

}
