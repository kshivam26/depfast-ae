
#include "service.h"
#include "server.h"
#include <thread>
# include <gperftools/profiler.h>

namespace janus {
thread_local bool hasPrinted = false;

FpgaRaftServiceImpl::FpgaRaftServiceImpl(TxLogServer *sched)
    : sched_((FpgaRaftServer*)sched) {
  Log_info("@@@ FpgaRaft CP 34: FpgaRaftServiceImpl::FpgaRaftServiceImpl");
	struct timespec curr_time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &curr_time);
	srand(curr_time.tv_nsec);
}

void FpgaRaftServiceImpl::Heartbeat(const uint64_t& leaderPrevLogIndex,
																		const DepId& dep_id,
																		uint64_t* followerPrevLogIndex,
																		rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 35: FpgaRaftServiceImpl::Heartbeat");
	//Log_info("received heartbeat");
	*followerPrevLogIndex = sched_->lastLogIndex;
	defer->reply();
}

void FpgaRaftServiceImpl::Forward(const MarshallDeputy& cmd,
                                    uint64_t* cmt_idx, 
                                    rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 36: FpgaRaftServiceImpl::Forward");
    Log_info("inside void FpgaRaftServiceImpl::Forward");
   verify(sched_ != nullptr);
   sched_->OnForward(const_cast<MarshallDeputy&>(cmd).sp_data_, cmt_idx,
                      std::bind(&rrr::DeferredReply::reply, defer));
    Log_info("returning from void FpgaRaftServiceImpl::Forward");

}

void FpgaRaftServiceImpl::Vote(const uint64_t& lst_log_idx,
                                    const ballot_t& lst_log_term,
                                    const parid_t& can_id,
                                    const ballot_t& can_term,
                                    ballot_t* reply_term,
                                    bool_t *vote_granted,
                                    rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 37: FpgaRaftServiceImpl::Vote");
  verify(sched_ != nullptr);
  sched_->OnVote(lst_log_idx,lst_log_term, can_id, can_term,
                    reply_term, vote_granted,
                    std::bind(&rrr::DeferredReply::reply, defer));
}

void FpgaRaftServiceImpl::Vote2FPGA(const uint64_t& lst_log_idx,
                                    const ballot_t& lst_log_term,
                                    const parid_t& can_id,
                                    const ballot_t& can_term,
                                    ballot_t* reply_term,
                                    bool_t *vote_granted,
                                    rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 38: FpgaRaftServiceImpl::Vote2FPGA");
  verify(sched_ != nullptr);
  sched_->OnVote2FPGA(lst_log_idx,lst_log_term, can_id, can_term,
                    reply_term, vote_granted,
                    std::bind(&rrr::DeferredReply::reply, defer));
}

void FpgaRaftServiceImpl::AppendEntries2(const uint64_t& slot,
                                        const ballot_t& ballot,
                                        const uint64_t& leaderCurrentTerm,
                                        const uint64_t& leaderPrevLogIndex,
                                        const uint64_t& leaderPrevLogTerm,
                                        const uint64_t& leaderCommitIndex,
																				const DepId& dep_id,
                                        const MarshallDeputy& md_cmd,
                                        uint64_t *followerAppendOK,
                                        uint64_t *followerCurrentTerm,
                                        uint64_t *followerLastLogIndex,
                                        rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 39: FpgaRaftServiceImpl::AppendEntries2");
	verify(sched_ != nullptr);
	*followerAppendOK = 1;
	defer->reply();

}

void FpgaRaftServiceImpl::AppendEntries(const uint64_t& slot,
                                        const ballot_t& ballot,
                                        const uint64_t& leaderCurrentTerm,
                                        const uint64_t& leaderPrevLogIndex,
                                        const uint64_t& leaderPrevLogTerm,
                                        const uint64_t& leaderCommitIndex,
																				const DepId& dep_id,
                                        const MarshallDeputy& md_cmd,
                                        uint64_t *followerAppendOK,
                                        uint64_t *followerCurrentTerm,
                                        uint64_t *followerLastLogIndex,
                                        rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 40: FpgaRaftServiceImpl::AppendEntries");
  verify(sched_ != nullptr);
  if (!hasPrinted) {
      Log_info("tid of non-leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }
	//Log_info("CreateRunning2");
  // Log_info("==== inside FpgaRaftServiceImpl::AppendEntries");


	/*if (ballot == 1000000000 || leaderPrevLogIndex + 1 < sched_->lastLogIndex) {
		*followerAppendOK = 1;
		*followerCurrentTerm = leaderCurrentTerm;
		*followerLastLogIndex = sched_->lastLogIndex + 1;
		/*for (int i = 0; i < 1000000; i++) {
			for (int j = 0; j < 1000; j++) {
				Log_info("wow: %d %d", leaderPrevLogIndex, sched_->lastLogIndex);
			}
		}
		defer->reply();
		return;
	}*/


  Coroutine::CreateRun([&] () {
    sched_->OnAppendEntries(slot,
                            ballot,
                            leaderCurrentTerm,
                            leaderPrevLogIndex,
                            leaderPrevLogTerm,
                            leaderCommitIndex,
														dep_id,
                            const_cast<MarshallDeputy&>(md_cmd).sp_data_,
                            followerAppendOK,
                            followerCurrentTerm,
                            followerLastLogIndex,
                            std::bind(&rrr::DeferredReply::reply, defer));  // #profile - 3.42%

  });

  // Log_info("==== returning from FpgaRaftServiceImpl::AppendEntries");
	
}

void FpgaRaftServiceImpl::Decide(const uint64_t& slot,
                                   const ballot_t& ballot,
																	 const DepId& dep_id,
                                   const MarshallDeputy& md_cmd,
                                   rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 41: FpgaRaftServiceImpl::Decide");
  // Log_info("**** inside FpgaRaftServiceImpl::Decide; tid: %d", gettid());
  verify(sched_ != nullptr);
	//Log_info("Deciding with string: %s and id: %d", dep_id.str.c_str(), dep_id.id);
  Coroutine::CreateRun([&] () {
    sched_->OnCommit(slot,
                     ballot,
                     const_cast<MarshallDeputy&>(md_cmd).sp_data_);
    defer->reply();
  });
}

void FpgaRaftServiceImpl::CrpcAppendEntries(const uint64_t& id, 
                        const uint64_t& slot, 
                        const ballot_t& ballot, 
                        const uint64_t& leaderCurrentTerm, 
                        const uint64_t& leaderPrevLogIndex, 
                        const uint64_t& leaderPrevLogTerm, 
                        const uint64_t& leaderCommitIndex, 
                        const DepId& dep_id, 
                        const MarshallDeputy& cmd, 
                        const std::vector<uint16_t>& addrChain, 
                        const std::vector<AppendEntriesResult>& state, 
                        rrr::DeferredReply* defer){
  Log_info("@@@ FpgaRaft CP 42: FpgaRaftServiceImpl::CrpcAppendEntries");
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
  // Log_info("*** inside FpgaRaftServiceImpl::CrpcAppendEntries; tid: %d", gettid());
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

  // Log_info("*** inside FpgaRaftServiceImpl::CrpcAppendEntries; cp 2 tid: %d", gettid());
  Coroutine::CreateRun([&] () {
      sched_->OnCRPC3(id,
                    slot, 
                    ballot, 
                    leaderCurrentTerm, 
                    leaderPrevLogIndex, 
                    leaderPrevLogTerm, 
                    leaderCommitIndex, 
                    dep_id, 
                    cmd,
                    addrChain,
                    state);
      // Log_info("*** inside FpgaRaftServiceImpl::CrpcAppendEntries; cp 3 tid: %d", gettid());
      defer->reply();
      Log_info("*** inside FpgaRaftServiceImpl::CrpcAppendEntries; cp 4 tid: %d; leaderPrevLogIndex: %d", gettid(), leaderPrevLogIndex);
  });

  // Log_info("*** returning from FpgaRaftServiceImpl::CrpcAppendEntries; tid: %d", gettid());
}

void FpgaRaftServiceImpl::CrpcAppendEntries3(const uint64_t& id, 
                          const uint64_t& slot, 
                          const ballot_t& ballot, 
                          const uint64_t& leaderCurrentTerm, 
                          const uint64_t& leaderPrevLogIndex, 
                          const uint64_t& leaderPrevLogTerm, 
                          const uint64_t& leaderCommitIndex, 
                          const DepId& dep_id, 
                          const MarshallDeputy& cmd, 
                          const std::vector<uint16_t>& addrChain, 
                          std::vector<AppendEntriesResult>* state, 
                          rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 43: FpgaRaftServiceImpl::CrpcAppendEntries3");
  // Log_info("$$$ inside FpgaRaftServiceImpl::CrpcAppendEntries3, calling sched_->OnCRPC_no_chain; tid is %d", gettid());
  verify(sched_ != nullptr);
  if (!hasPrinted) {
      Log_info("tid of non-leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }

  Coroutine::CreateRun([&] () {
      sched_->OnCRPC_no_chain(id,
                    slot, 
                    ballot, 
                    leaderCurrentTerm, 
                    leaderPrevLogIndex, 
                    leaderPrevLogTerm, 
                    leaderCommitIndex, 
                    dep_id, 
                    cmd,
                    addrChain,
                    state, std::bind(&rrr::DeferredReply::reply, defer));
      // Log_info("$$$ inside FpgaRaftServiceImpl::CrpcAppendEntries3, returned from calling sched_->OnCRPC_no_chain; tid is %d", gettid());
      // defer->reply();
  });

}

void FpgaRaftServiceImpl::cRPC(const uint64_t& id,
                              const MarshallDeputy& cmd, 
                              const std::vector<uint16_t>& addrChain, 
                              const MarshallDeputy& state, 
                              rrr::DeferredReply* defer) {
  Log_info("@@@ FpgaRaft CP 44: FpgaRaftServiceImpl::cRPC");
  Log_info("==== inside void FpgaRaftServiceImpl::cRPC");

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

} // namespace janus;
