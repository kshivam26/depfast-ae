

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
  void SampleCrpcServer::OnAdd(const int64_t value1,
                                     const int64_t value2,
                                     int64_t *resultSum,
                                     const function<void()> &cb) {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        //StartTimer() ;
        // Log_info("==== inside void SampleCrpcServer::OnAdd");
        

            *resultSum = value1 + value2;
            //Log_info("*** --- SampleCrpcServer::OnAdd sum is: %ld + %ld = %ld, tid: %d", value1, value2, *resultSum, gettid());
						// if (cmd->kind_ == MarshallDeputy::CMD_TPC_COMMIT){
            //   auto p_cmd = dynamic_pointer_cast<TpcCommitCommand>(cmd);
            //   auto sp_vec_piece = dynamic_pointer_cast<VecPieceData>(p_cmd->cmd_)->sp_vec_piece_data_;
              
						// 	vector<struct KeyValue> kv_vector;
						// 	int index = 0;
						// 	for (auto it = sp_vec_piece->begin(); it != sp_vec_piece->end(); it++){
						// 		auto cmd_input = (*it)->input.values_;
						// 		for (auto it2 = cmd_input->begin(); it2 != cmd_input->end(); it2++) {
						// 			struct KeyValue key_value = {it2->first, it2->second.get_i32()};
						// 			kv_vector.push_back(key_value);
						// 		}
						// 	}

						// 	struct KeyValue key_values[kv_vector.size()];
						// 	std::copy(kv_vector.begin(), kv_vector.end(), key_values);

						// 	auto de = IO::write(filename, key_values, sizeof(struct KeyValue), kv_vector.size()); // ***uncomment, testing no IO
						// 	de->Wait();
            // } else {
						// 	int value = -1;
						// 	auto de = IO::write(filename, &value, sizeof(int), 1); // ***uncomment, testing no IO
            //   de->Wait();
            // }

				/*if (rand() % 1000 == 0) {
					usleep(25*1000);
				}*/
        cb();  // #profile(n_crpc) - 1.15%
        // Log_info("==== returning from void SampleCrpcServer::OnAdd");
    }


  void SampleCrpcServer::OnCRPC3(const uint64_t& id,
              const int64_t& value1,
              const int64_t& value2,
              const std::vector<uint16_t>& addrChain, 
              const std::vector<ResultAdd>& state){
    //Log_info("*** inside SampleCrpcServer::OnCRPC; cp 1 tid: %d", gettid());
    if (addrChain.size() == 1)
    {
        // Log_info("==== reached the final link in the chain");
        // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 1 @ %d", gettid());
        // // add a verify statement
        auto x = (SampleCrpcCommo *)(this->commo_);
        verify(x->cRPCEvents.find(id) != x->cRPCEvents.end()); // #profile - 1.40%
        // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 2 @ %d", gettid());
        auto ev = x->cRPCEvents[id];
        x->cRPCEvents.erase(id);

        // Log_info("==== inside demoserviceimpl::cRPC; results state is following");
        // auto st = dynamic_pointer_cast<AddCommandState>(state.sp_data_);   // #profile - 0.54%
        for (size_t i = 0; i < state.size(); ++i)
        {
          // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 3 @ %d", gettid());
          bool y = true;
          ev->FeedResponse(y, i);
        }
        // Log_info("inside SampleCrpcServer::OnCRPC2; checkpoint 4 @ %d", gettid());
        // Log_info("==== returning from cRPC");
        return;
    }

    // Log_info("calling dynamic_pointer_cast<AddCommand>(state.sp_data_)");
    // auto c = dynamic_pointer_cast<AddCommand>(cmd.sp_data_);
    // Log_info("return dynamic_pointer_cast<AddCommand>(state.sp_data_)");
    ResultAdd res;
    auto r = Coroutine::CreateRun([&]()
                                  { this->OnAdd(value1,
                                                          value2,
                                                          // const_cast<MarshallDeputy &>(cmd).sp_data_,
                                                          &res.result,
                                                          []() {}); }); // #profile - 2.88%
    // Log_info("###################cp1");
    // this->OnAdd(slot_id,
    //                               ballot,
    //                               leaderCurrentTerm,
    //                               leaderPrevLogIndex,
    //                               leaderPrevLogTerm,
    //                               leaderCommitIndex,
    //                               dep_id,
    //                               const_cast<MarshallDeputy &>(cmd).sp_data_,
    //                               &res.followerAppendOK,
    //                               &res.followerCurrentTerm,
    //                               &res.followerLastLogIndex,
    //                               []() {});
    // Log_info("###################cp2");
    // Log_info("calling dynamic_pointer_cast<AddCommandState>(state.sp_data_)");
    std::vector<ResultAdd> st(state);
    // auto st = dynamic_pointer_cast<AddCommandState>(state.sp_data_);  // #profile - 1.23%  ==> dont think can do anything about it
    // Log_info("returned dynamic_pointer_cast<AddCommandState>(state.sp_data_)");
    st.push_back(res);

    vector<uint16_t> addrChainCopy(addrChain.begin() + 1, addrChain.end());
    // auto addrChainCopy = addrChain;
    // addrChainCopy.erase(addrChainCopy.begin());
    // Log_info("inside SampleCrpcServer::OnCRPC3; calling CrpcAdd3");
    // Log_info("*** inside SampleCrpcServer::OnCRPC; cp 2 tid: %d", gettid());
    parid_t par_id = this->frame_->site_info_->partition_id_;
    ((SampleCrpcCommo *)(this->commo_))->CrpcAdd3(par_id, id, 
                                                          value1,
                                                          value2,
                                                          addrChainCopy, st); // #profile (crpc2) - 4.02%%
                                                                                              // Log_info("==== returning from void SampleCrpcServer::OnCRPC");
    // Log_info("*** inside SampleCrpcServer::OnCRPC; cp 3 tid: %d", gettid());
  }

} // namespace janus
