
#include "commo.h"
#include "../rcc/graph.h"
#include "../rcc/graph_marshaler.h"
#include "../command.h"
#include "../procedure.h"
#include "../command_marshaler.h"
#include "../rcc_rpc.h"
#include "append_entries_command.h"

namespace janus {

FpgaRaftCommo::FpgaRaftCommo(PollMgr* poll) : Communicator(poll) {
  Log_info("@@@ FpgaRaft CP 16: FpgaRaftCommo::FpgaRaftCommo");
//  verify(poll != nullptr);
}

shared_ptr<FpgaRaftForwardQuorumEvent> FpgaRaftCommo::SendForward(parid_t par_id, 
                                            parid_t self_id, shared_ptr<Marshallable> cmd)
{
  Log_info("@@@ FpgaRaft CP 17: FpgaRaftCommo::SendForward");
    int n = Config::GetConfig()->GetPartitionSize(par_id);
    auto e = Reactor::CreateSpEvent<FpgaRaftForwardQuorumEvent>(1,1);
    parid_t fid = (self_id + 1 ) % n ;
    if (fid != self_id + 1 )
    {
      // sleep for 2 seconds cos no leader
      int32_t timeout = 2*1000*1000 ;
      auto sp_e = Reactor::CreateSpEvent<TimeoutEvent>(timeout);
      sp_e->Wait();    
    }
    auto proxies = rpc_par_proxies_[par_id];
    WAN_WAIT;
    auto proxy = (FpgaRaftProxy*) proxies[fid].second ;
    FutureAttr fuattr;
    fuattr.callback = [e](Future* fu) {
      uint64_t cmt_idx = 0;
      fu->get_reply() >> cmt_idx;
      e->FeedResponse(cmt_idx);
    };    
    MarshallDeputy md(cmd);
    auto f = proxy->async_Forward(md, fuattr);
    Future::safe_release(f);
    return e;
}

void FpgaRaftCommo::BroadcastHeartbeat(parid_t par_id,
																			 uint64_t logIndex) {
  Log_info("@@@ FpgaRaft CP 18: FpgaRaftCommo::BroadcastHeartbeat");
	//Log_info("heartbeat for log index: %d", logIndex);
  Log_info("inside FpgaRaftCommo::BroadcastHeartbeat");
  auto proxies = rpc_par_proxies_[par_id];
  vector<Future*> fus;
  for (auto& p : proxies) {
    if (p.first == this->loc_id_)
        continue;
		auto follower_id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    
		fuattr.callback = [this, follower_id, logIndex] (Future* fu) {
      uint64_t index = 0;
			
      fu->get_reply() >> index;
			this->matchedIndex[follower_id] = index;
			
			//Log_info("follower_index for %d: %d and leader_index: %d", follower_id, index, logIndex);
			
    };

		DepId di;
		di.str = "hb";
		di.id = -1;
    auto f = proxy->async_Heartbeat(logIndex, di, fuattr);
    Future::safe_release(f);
    // // Log_info("*** returning from FpgaRaftCommo::BroadcastHeartbeat");
  }
}

void FpgaRaftCommo::SendHeartbeat(parid_t par_id,
																	siteid_t site_id,
																  uint64_t logIndex) {
  Log_info("@@@ FpgaRaft CP 19: FpgaRaftCommo::SendHeartbeat");
  auto proxies = rpc_par_proxies_[par_id];
  vector<Future*> fus;
	WAN_WAIT;
  for (auto& p : proxies) {
    if (p.first != site_id)
        continue;
		auto follower_id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    fuattr.callback = [](Future* fu) {};
    
		DepId di;
		di.str = "dep";
		di.id = -1;
		
		//Log_info("heartbeat2 for log index: %d", logIndex);
    auto f = proxy->async_Heartbeat(logIndex, di, fuattr);
    Future::safe_release(f);
  }
}

void FpgaRaftCommo::SendAppendEntriesAgain(siteid_t site_id,
																					 parid_t par_id,
																					 slotid_t slot_id,
																					 ballot_t ballot,
																					 bool isLeader,
																					 uint64_t currentTerm,
																					 uint64_t prevLogIndex,
																					 uint64_t prevLogTerm,
																					 uint64_t commitIndex,
																					 shared_ptr<Marshallable> cmd) {
  Log_info("@@@ FpgaRaft CP 20: FpgaRaftCommo::SendAppendEntriesAgain");
  auto proxies = rpc_par_proxies_[par_id];
  vector<Future*> fus;
	WAN_WAIT;
  for (auto& p : proxies) {
    if (p.first != site_id)
        continue;
		auto follower_id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    fuattr.callback = [](Future* fu) {};

		MarshallDeputy md(cmd);
		verify(md.sp_data_ != nullptr);

		DepId di;
		di.str = "dep";
		di.id = -1;

		Log_info("heartbeat2 for log index: %d", prevLogIndex);
    auto f = proxy->async_AppendEntries(slot_id,
                                        ballot,
                                        currentTerm,
                                        prevLogIndex,
                                        prevLogTerm,
                                        commitIndex,
																				di,
                                        md, 
                                        fuattr);
    Future::safe_release(f);
  }

}

// // testing cRPC on broadcastAppendEntries
// shared_ptr<FpgaRaftAppendQuorumEvent>
// FpgaRaftCommo::BroadcastAppendEntries(parid_t par_id,
//                                       siteid_t leader_site_id,
//                                       slotid_t slot_id,
//                                       i64 dep_id,
//                                       ballot_t ballot,
//                                       bool isLeader,
//                                       uint64_t currentTerm,
//                                       uint64_t prevLogIndex,
//                                       uint64_t prevLogTerm,
//                                       uint64_t commitIndex,
//                                       shared_ptr<Marshallable> cmd) {
//   // // Log_info("*** inside FpgaRaftCommo::BroadcastAppendEntries");
//   // Log_info("1) the pid for leader is: %d", ::getpid());
//   // Log_info("2) the thread id for leader is: %d", gettid());

//   static bool hasPrinted = false;  // Static variable to track if it has printed

//   if (!hasPrinted) {
//       Log_info("in CRPC; tid of leader is %d", gettid());
//       hasPrinted = true;  // Update the static variable
//   }

//   int n = Config::GetConfig()->GetPartitionSize(par_id);
//   auto e = Reactor::CreateSpEvent<FpgaRaftAppendQuorumEvent>(n, n/2 + 1);
//   auto proxies = rpc_par_proxies_[par_id];

//   unordered_set<std::string> ip_addrs {};
//   std::vector<std::shared_ptr<rrr::Client>> clients;

//   WAN_WAIT;
//   std::vector<uint16_t> sitesInfo_; // additional; looks like can be computed in cRPC call
  
//   for (auto& p : proxies) {
//     auto id = p.first;
//     auto proxy = (FpgaRaftProxy*) p.second;
//     auto cli_it = rpc_clients_.find(id);
//     std::string ip = "";
//     if (cli_it != rpc_clients_.end()) {
//       ip = cli_it->second->host();
//     }
//     ip_addrs.insert(ip);
//     if (id != leader_site_id) { // #cPRC additional
//       sitesInfo_.push_back(id); // #cPRC additional
//     }                           // #cPRC additional
// 		//clients.push_back(cli);
//   }
//   //e->clients_ = clients;

//   // TODO: remove line below, adding this to shorten the chain
//   // sitesInfo_.pop_back();

//   sitesInfo_.push_back(leader_site_id); // #cPRC additional

//   MarshallDeputy aes_md(dynamic_pointer_cast<Marshallable>(std::make_shared<AppendEntriesCommandState>())); // additional
//   for (auto& p : proxies) {    
//     auto follower_id = p.first;
//     auto proxy = (FpgaRaftProxy*) p.second;
//     auto cli_it = rpc_clients_.find(follower_id);
//     std::string ip = "";
//     if (cli_it != rpc_clients_.end()) {
//       ip = cli_it->second->host();
//     }
// 	  if (p.first == leader_site_id) {
//         // fix the 1c1s1p bug
//         // Log_info("leader_site_id %d", leader_site_id);
//         e->FeedResponse(true, prevLogIndex + 1, ip);
//         continue;
//     }

//     // // Log_info("*** inside broadcastAppendEntries; ip is: %s", ip.c_str());
//     FutureAttr fuattr;  
//     struct timespec begin;
//     clock_gettime(CLOCK_MONOTONIC, &begin);

//     fuattr.callback = [this, e, isLeader, currentTerm, follower_id, n, ip, begin] (Future* fu) {
//       // // Log_info("*** inside FpgaRaftCommo::BroadcastAppendEntries; received response"); 
//     };
//     MarshallDeputy md(cmd);
// 		verify(md.sp_data_ != nullptr);
// 		outbound++;
// 		DepId di;
// 		di.str = "dep";
// 		di.id = dep_id;
//  // // Log_info("*** inside FpgaRaftCommo::BroadcastAppendEntries; calling proxy->async_AppendEntries");
    
//     // Log_info("calling std::make_shared<AppendEntriesCommand>");
//     // #cPRC additional conversion here
//     // #cPRC not abstracted from the user
//     auto ae_cmd = std::make_shared<AppendEntriesCommand>(slot_id, 
//                                                         ballot, 
//                                                         currentTerm, 
//                                                         prevLogIndex, 
//                                                         prevLogTerm, 
//                                                         commitIndex, 
//                                                         di, 
//                                                         md); // call missing just fuattr parameter, everything else same

//     // Log_info("returning std::make_shared<AppendEntriesCommand>");

//     MarshallDeputy ae_md(dynamic_pointer_cast<Marshallable>(ae_cmd));
    
//     // crpc_id generation is also not abstracted
//     uint64_t crpc_id = reinterpret_cast<uint64_t>(&e);
//     // // Log_info("*** crpc_id is: %d", crpc_id); // verify it's never the same
//     verify(cRPCEvents.find(crpc_id) == cRPCEvents.end());

//     // just call cRPC something with the above paramters, and no other changes
//     auto f = proxy->async_cRPC(crpc_id, ae_md, sitesInfo_, aes_md, fuattr); // this can definitely be pushed into the cRPC function below // #profile - 0.83%
//     Future::safe_release(f);

//     // this too should be abstracted
//     cRPCEvents[crpc_id] = e;

//     // rather than breaking, do something else; when iterating through proxies
//     break;

//     }
//   verify(!e->IsReady());
//   // // Log_info("*** returning from FpgaRaftCommo::BroadcastAppendEntries");
//   return e;
// }


// // testing crpcAppendEntries
shared_ptr<FpgaRaftAppendQuorumEvent>
FpgaRaftCommo::crpc_ring_BroadcastAppendEntries(parid_t par_id,
                                      siteid_t leader_site_id,
                                      slotid_t slot_id,
                                      i64 dep_id,
                                      ballot_t ballot,
                                      bool isLeader,
                                      uint64_t currentTerm,
                                      uint64_t prevLogIndex,
                                      uint64_t prevLogTerm,
                                      uint64_t commitIndex,
                                      shared_ptr<Marshallable> cmd) {
  Log_info("@@@ FpgaRaft CP 21: FpgaRaftCommo::crpc_ring_BroadcastAppendEntries");
  static bool hasPrinted = false;  // Static variable to track if it has printed
  if (!hasPrinted) {
      Log_info("In crpcAppendEntries_ring_back; tid of leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }

  int n = Config::GetConfig()->GetPartitionSize(par_id);
  auto e = Reactor::CreateSpEvent<FpgaRaftAppendQuorumEvent>(n, n/2 + 1);
  auto proxies = rpc_par_proxies_[par_id];

  unordered_set<std::string> ip_addrs {};
  std::vector<std::shared_ptr<rrr::Client>> clients;

  WAN_WAIT;
  std::vector<uint16_t> sitesInfo_; // additional; looks like can be computed in cRPC call
  
  for (auto& p : proxies) {
    auto id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    auto cli_it = rpc_clients_.find(id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();
    }
    ip_addrs.insert(ip);
    if (id != leader_site_id) { // #cPRC additional
      sitesInfo_.push_back(id); // #cPRC additional
    }                           // #cPRC additional
		//clients.push_back(cli);
  }
  //e->clients_ = clients;

  // TODO: remove line below, adding this to shorten the chain
  // sitesInfo_.pop_back();

  sitesInfo_.push_back(leader_site_id); // #cPRC additional

  // MarshallDeputy aes_md(dynamic_pointer_cast<Marshallable>(std::make_shared<AppendEntriesCommandState>())); // additional
  for (auto& p : proxies) {    
    auto follower_id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    auto cli_it = rpc_clients_.find(follower_id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();
    }
	  if (p.first == leader_site_id) {
        // fix the 1c1s1p bug
        // Log_info("leader_site_id %d", leader_site_id);
        e->FeedResponse(true, prevLogIndex + 1, ip);
        continue;
    }

    MarshallDeputy md(cmd);
		verify(md.sp_data_ != nullptr);
		outbound++;
		DepId di;
		di.str = "dep";
		di.id = dep_id;

    // auto ae_cmd = std::make_shared<AppendEntriesCommand>(slot_id, 
    //                                                     ballot, 
    //                                                     currentTerm, 
    //                                                     prevLogIndex, 
    //                                                     prevLogTerm, 
    //                                                     commitIndex, 
    //                                                     di, 
    //                                                     md); // call missing just fuattr parameter, everything else same

    // Log_info("returning std::make_shared<AppendEntriesCommand>");

    // MarshallDeputy ae_md(dynamic_pointer_cast<Marshallable>(ae_cmd));
    
    // crpc_id generation is also not abstracted
    uint64_t crpc_id = reinterpret_cast<uint64_t>(&e);
    // // Log_info("*** crpc_id is: %d", crpc_id); // verify it's never the same
    verify(cRPCEvents.find(crpc_id) == cRPCEvents.end());

    std::vector<AppendEntriesResult> state;

    // **** uncomment/comment the fuattr; just testing if ringback is even more costly
    // FutureAttr fuattr;

    // fuattr.callback = [this, e, isLeader, currentTerm, follower_id, n, ip] (Future* fu) {
    //   Log_info("*** inside fuattr.callback, response received; tid is %d", gettid());
    // };
    // just call cRPC something with the above paramters, and no other changes
    auto f = proxy->async_CrpcAppendEntries(crpc_id,slot_id, 
                                                        ballot, 
                                                        currentTerm, 
                                                        prevLogIndex, 
                                                        prevLogTerm, 
                                                        commitIndex, 
                                                        di, 
                                                        md, sitesInfo_, state); // this can definitely be pushed into the cRPC function below // #profile (crpc2) - 2.05%
    Future::safe_release(f);

    // this too should be abstracted
    cRPCEvents[crpc_id] = e;

    // rather than breaking, do something else; when iterating through proxies
    break;

    }
  verify(!e->IsReady());
  // Log_info("*** returning from FpgaRaftCommo::crpc_ring_BroadcastAppendEntries");
  return e;
}


// testing crpcAppendEntries no chain 
shared_ptr<FpgaRaftAppendQuorumEvent>
FpgaRaftCommo::crpc_BroadcastAppendEntries(parid_t par_id,
                                      siteid_t leader_site_id,
                                      slotid_t slot_id,
                                      i64 dep_id,
                                      ballot_t ballot,
                                      bool isLeader,
                                      uint64_t currentTerm,
                                      uint64_t prevLogIndex,
                                      uint64_t prevLogTerm,
                                      uint64_t commitIndex,
                                      shared_ptr<Marshallable> cmd) {
  Log_info("@@@ FpgaRaft CP 22: FpgaRaftCommo::crpc_BroadcastAppendEntries");
  static bool hasPrinted = false;  // Static variable to track if it has printed
  if (!hasPrinted) {
      Log_info("In crpcAppendEntries_no_chain; tid of leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }

  // Log_info("*** inside void FpgaRaftCommo::BroadcastAppendEntries; slot_id: %ld", slot_id);

  int n = Config::GetConfig()->GetPartitionSize(par_id);
  auto e = Reactor::CreateSpEvent<FpgaRaftAppendQuorumEvent>(n, n/2 + 1);
  auto proxies = rpc_par_proxies_[par_id];

  unordered_set<std::string> ip_addrs {};
  std::vector<std::shared_ptr<rrr::Client>> clients;

  WAN_WAIT;
  std::vector<uint16_t> sitesInfo_; // additional; looks like can be computed in cRPC call
  
  for (auto& p : proxies) {
    auto id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    auto cli_it = rpc_clients_.find(id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();
    }
    ip_addrs.insert(ip);
    if (id != leader_site_id) { // #cPRC additional
      sitesInfo_.push_back(id); // #cPRC additional
    }                           // #cPRC additional
		//clients.push_back(cli);
  }

  // MarshallDeputy aes_md(dynamic_pointer_cast<Marshallable>(std::make_shared<AppendEntriesCommandState>())); // additional
  for (auto& p : proxies) {    
    auto follower_id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    auto cli_it = rpc_clients_.find(follower_id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();
    }
	  if (p.first == leader_site_id) {
        // fix the 1c1s1p bug
        // Log_info("leader_site_id %d", leader_site_id);
        e->FeedResponse(true, prevLogIndex + 1, ip);
        continue;
    }

    FutureAttr fuattr;

    fuattr.callback = [this, e, isLeader, currentTerm, follower_id, n, ip] (Future* fu) {
      // Log_info("*** inside fuattr.callback, response received; tid is %d", gettid());

      vector<AppendEntriesResult> result;			
      fu->get_reply() >> result;

			// struct timespec end;
			//clock_gettime(CLOCK_MONOTONIC, &begin);
			// this->outbound--;
			//Log_info("reply from server: %s and is_ready: %d", ip.c_str(), e->IsReady());
			// clock_gettime(CLOCK_MONOTONIC, &end);
			//Log_info("time of reply on server %d: %ld", follower_id, (end.tv_sec - begin.tv_sec)*1000000000 + end.tv_nsec - begin.tv_nsec);

			for (auto r: result){
        // Log_info("$$$ inside fuattr.callback, some result; tid is %d", gettid());
        bool y = ((r.followerAppendOK == 1) && (isLeader) && (currentTerm == r.followerCurrentTerm));
        e->FeedResponse(y, r.followerLastLogIndex, ip);
      }      
    };

    MarshallDeputy md(cmd);
		verify(md.sp_data_ != nullptr);
		outbound++;
		DepId di;
		di.str = "dep";
		di.id = dep_id;

    // Log_info("$$$ inside BroadcastAppendEntries; calling async_CrpcAppendEntries3; tid is %d", gettid());
    // just call cRPC something with the above paramters, and no other changes
    auto f = proxy->async_CrpcAppendEntries3(1, slot_id, 
                                                        ballot, 
                                                        currentTerm, 
                                                        prevLogIndex, 
                                                        prevLogTerm, 
                                                        commitIndex, 
                                                        di, 
                                                        md, sitesInfo_, fuattr); // this can definitely be pushed into the cRPC function below // #profile (crpc2) - 2.05%
    Future::safe_release(f);

    // rather than breaking, do something else; when iterating through proxies
    break;
  }
  verify(!e->IsReady());
  // // Log_info("*** returning from FpgaRaftCommo::BroadcastAppendEntries");
  return e;
}



// // // previous no cRPC
shared_ptr<FpgaRaftAppendQuorumEvent>
FpgaRaftCommo::BroadcastAppendEntries(parid_t par_id,
                                      siteid_t leader_site_id,
                                      slotid_t slot_id,
                                      i64 dep_id,
                                      ballot_t ballot,
                                      bool isLeader,
                                      uint64_t currentTerm,
                                      uint64_t prevLogIndex,
                                      uint64_t prevLogTerm,
                                      uint64_t commitIndex,
                                      shared_ptr<Marshallable> cmd) {
  Log_info("@@@ FpgaRaft CP 23: FpgaRaftCommo::BroadcastAppendEntries");
  static bool hasPrinted = false;  // Static variable to track if it has printed

  if (!hasPrinted) {
      Log_info("in no cRPC; tid of leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }
  // static uint64_t count = 0;
  // count++;
  // // Log_info("*** inside void FpgaRaftCommo::BroadcastAppendEntries; count: %ld", count);
  // Log_info("*** inside void FpgaRaftCommo::BroadcastAppendEntries; slot_id: %ld; tid is: %d", slot_id, gettid());

  int n = Config::GetConfig()->GetPartitionSize(par_id);
  auto e = Reactor::CreateSpEvent<FpgaRaftAppendQuorumEvent>(n, n/2 + 1);
  auto proxies = rpc_par_proxies_[par_id];

  unordered_set<std::string> ip_addrs {};
  std::vector<std::shared_ptr<rrr::Client>> clients;

  vector<Future*> fus;
  WAN_WAIT;

  for (auto& p : proxies) {
    auto id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    auto cli_it = rpc_clients_.find(id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();
			//cli = cli_it->second;
    }
    ip_addrs.insert(ip);
		//clients.push_back(cli);
  }
  //e->clients_ = clients;
  
  for (auto& p : proxies) {
    auto follower_id = p.first;
    auto proxy = (FpgaRaftProxy*) p.second;
    auto cli_it = rpc_clients_.find(follower_id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();  
    }
	if (p.first == leader_site_id) {
        // fix the 1c1s1p bug
        // Log_info("leader_site_id %d", leader_site_id);
        
        e->FeedResponse(true, prevLogIndex + 1, ip);
        continue;
    }
    FutureAttr fuattr;
    struct timespec begin;
    clock_gettime(CLOCK_MONOTONIC, &begin);

    fuattr.callback = [this, e, isLeader, currentTerm, follower_id, n, ip, begin] (Future* fu) {
      // Log_info("$$$ inside fuattr.callback, response received; count: %ld", count);
      // Log_info("*** inside FpgaRaftCommo::BroadcastAppendEntries; received response");
      uint64_t accept = 0;
      uint64_t term = 0;
      uint64_t index = 0;
			
			fu->get_reply() >> accept;
      fu->get_reply() >> term;
      fu->get_reply() >> index;
			
			struct timespec end;
			//clock_gettime(CLOCK_MONOTONIC, &begin);
			this->outbound--;
			//Log_info("reply from server: %s and is_ready: %d", ip.c_str(), e->IsReady());
			clock_gettime(CLOCK_MONOTONIC, &end);
			//Log_info("time of reply on server %d: %ld", follower_id, (end.tv_sec - begin.tv_sec)*1000000000 + end.tv_nsec - begin.tv_nsec);
			
      bool y = ((accept == 1) && (isLeader) && (currentTerm == term));
      e->FeedResponse(y, index, ip);
    };
    MarshallDeputy md(cmd);
		verify(md.sp_data_ != nullptr);
		outbound++;
		DepId di;
		di.str = "dep";
		di.id = dep_id;
    // // Log_info("*** inside FpgaRaftCommo::BroadcastAppendEntries; calling proxy->async_AppendEntries");
    auto f = proxy->async_AppendEntries(slot_id,
                                        ballot,
                                        currentTerm,
                                        prevLogIndex,
                                        prevLogTerm,
                                        commitIndex,
																				di,
                                        md, 
                                        fuattr); // #profile - 1.36%
    Future::safe_release(f);
  }
  verify(!e->IsReady());
  // // Log_info("*** returning from FpgaRaftCommo::BroadcastAppendEntries");
  return e;
}

void FpgaRaftCommo::BroadcastAppendEntries(parid_t par_id,
                                           slotid_t slot_id,
																					 i64 dep_id,
                                           ballot_t ballot,
                                           uint64_t currentTerm,
                                           uint64_t prevLogIndex,
                                           uint64_t prevLogTerm,
                                           uint64_t commitIndex,
                                           shared_ptr<Marshallable> cmd,
                                           const function<void(Future*)>& cb) {
  Log_info("@@@ FpgaRaft CP 24: FpgaRaftCommo::BroadcastAppendEntries");
  verify(0); // deprecated function
  auto proxies = rpc_par_proxies_[par_id];
  vector<Future*> fus;
  for (auto& p : proxies) {
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    fuattr.callback = cb;
    MarshallDeputy md(cmd);
		DepId di;
		di.str = "dep";
		di.id = dep_id;
    auto f = proxy->async_AppendEntries(slot_id, 
                                        ballot, 
                                        currentTerm,
                                        prevLogIndex,
                                        prevLogTerm,
                                        commitIndex,
																				di,
                                        md, 
                                        fuattr);
    Future::safe_release(f);
  }
//  verify(0);
}

void FpgaRaftCommo::BroadcastDecide(const parid_t par_id,
                                      const slotid_t slot_id,
																			const i64 dep_id,
                                      const ballot_t ballot,
                                      const shared_ptr<Marshallable> cmd) {
  Log_info("@@@ FpgaRaft CP 25: FpgaRaftCommo::BroadcastDecide");
  auto proxies = rpc_par_proxies_[par_id];
  vector<Future*> fus;
  for (auto& p : proxies) {
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    fuattr.callback = [](Future* fu) {};
    MarshallDeputy md(cmd);
		DepId di;
		di.str = "dep";
		di.id = dep_id;
    auto f = proxy->async_Decide(slot_id, ballot, di, md, fuattr);
    Future::safe_release(f);
  }
}

void FpgaRaftCommo::BroadcastVote(parid_t par_id,
                                        slotid_t lst_log_idx,
                                        ballot_t lst_log_term,
                                        parid_t self_id,
                                        ballot_t cur_term,
                                       const function<void(Future*)>& cb) {
  Log_info("@@@ FpgaRaft CP 26: FpgaRaftCommo::BroadcastVote");
  verify(0); // deprecated function
  auto proxies = rpc_par_proxies_[par_id];
  for (auto& p : proxies) {
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    fuattr.callback = cb;
    Future::safe_release(proxy->async_Vote(lst_log_idx, lst_log_term, self_id,cur_term, fuattr));
  }
}

shared_ptr<FpgaRaftVoteQuorumEvent>
FpgaRaftCommo::BroadcastVote(parid_t par_id,
                                    slotid_t lst_log_idx,
                                    ballot_t lst_log_term,
                                    parid_t self_id,
                                    ballot_t cur_term ) {
  Log_info("@@@ FpgaRaft CP 27: FpgaRaftCommo::BroadcastVote");
  // // Log_info("*** inside FpgaRaftCommo::BroadcastVote");
  int n = Config::GetConfig()->GetPartitionSize(par_id);
  auto e = Reactor::CreateSpEvent<FpgaRaftVoteQuorumEvent>(n, n/2);
  auto proxies = rpc_par_proxies_[par_id];
  WAN_WAIT;
  for (auto& p : proxies) {
    if (p.first == this->loc_id_)
        continue;
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    fuattr.callback = [e](Future* fu) {
      // // Log_info("*** received response in vote in fuattr");
      ballot_t term = 0;
      bool_t vote = false ;
      fu->get_reply() >> term;
      fu->get_reply() >> vote ;
      e->FeedResponse(vote, term);
      // TODO add max accepted value.
    };
    Future::safe_release(proxy->async_Vote(lst_log_idx, lst_log_term, self_id, cur_term, fuattr));
  }
  // // Log_info("*** returning from FpgaRaftCommo::BroadcastVote");
  return e;
}

void FpgaRaftCommo::BroadcastVote2FPGA(parid_t par_id,
                                        slotid_t lst_log_idx,
                                        ballot_t lst_log_term,
                                        parid_t self_id,
                                        ballot_t cur_term,
                                       const function<void(Future*)>& cb) {
  Log_info("@@@ FpgaRaft CP 28: FpgaRaftCommo::BroadcastVote2FPGA");
  verify(0); // deprecated function
  auto proxies = rpc_par_proxies_[par_id];
  for (auto& p : proxies) {
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    fuattr.callback = cb;
    Future::safe_release(proxy->async_Vote(lst_log_idx, lst_log_term, self_id,cur_term, fuattr));
  }
}

shared_ptr<FpgaRaftVote2FPGAQuorumEvent>
FpgaRaftCommo::BroadcastVote2FPGA(parid_t par_id,
                                    slotid_t lst_log_idx,
                                    ballot_t lst_log_term,
                                    parid_t self_id,
                                    ballot_t cur_term ) {
  Log_info("@@@ FpgaRaft CP 29: FpgaRaftCommo::BroadcastVote2FPGA");
  int n = Config::GetConfig()->GetPartitionSize(par_id);
  auto e = Reactor::CreateSpEvent<FpgaRaftVote2FPGAQuorumEvent>(n, n/2);
  auto proxies = rpc_par_proxies_[par_id];
  WAN_WAIT;
  for (auto& p : proxies) {
    if (p.first == this->loc_id_)
        continue;
    auto proxy = (FpgaRaftProxy*) p.second;
    FutureAttr fuattr;
    fuattr.callback = [e](Future* fu) {
      ballot_t term = 0;
      bool_t vote = false ;
      fu->get_reply() >> term;
      fu->get_reply() >> vote ;
      e->FeedResponse(vote, term);
    };
    Future::safe_release(proxy->async_Vote(lst_log_idx, lst_log_term, self_id, cur_term, fuattr));
  }
  return e;
}

void FpgaRaftCommo::cRPC(const parid_t par_id,
              const uint64_t& id,
              const MarshallDeputy& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const MarshallDeputy& state){
  Log_info("@@@ FpgaRaft CP 30: FpgaRaftCommo::cRPC");
  // // Log_info("*** inside FpgaRaftCommo::cRPC");

  FutureAttr fuattr;
  fuattr.callback = [](Future *fu)
  {
    // // Log_info("*** received a response back, in cRPC");
  };

  auto proxies = rpc_par_proxies_[par_id];
  FpgaRaftProxy *proxy = nullptr;

  proxy = (FpgaRaftProxy *)rpc_proxies_[addrChain[0]];
  // for (auto p : proxies)          // #cPRC TODO: room for optimization?
  // {
  //   if (p.first == addrChain[0])
  //   {
  //       proxy = (FpgaRaftProxy *)p.second;
  //       break;
  //   }
  // }

  // // Log_info("*** calling proxy->proxy->async_cRPC");
  auto f = proxy->async_cRPC(id, cmd, addrChain, state, fuattr);  // #profile - 1.57%
  Future::safe_release(f);
}


void FpgaRaftCommo::CrpcAppendEntries(const parid_t par_id,
              const uint64_t& id,
              const AppendEntriesCommand& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const std::vector<AppendEntriesResult>& state){
  Log_info("@@@ FpgaRaft CP 31: FpgaRaftCommo::CrpcAppendEntries");
  // Log_info("inside FpgaRaftCommo::CrpcAppendEntries; checkpoint 0 @ %d", gettid());
  auto proxies = rpc_par_proxies_[par_id];
  FpgaRaftProxy *proxy = nullptr;

  proxy = (FpgaRaftProxy *)rpc_proxies_[addrChain[0]];
  auto f = proxy->async_CrpcAppendEntries(id, cmd.slot, cmd.ballot, cmd.leaderCurrentTerm, cmd.leaderPrevLogIndex, cmd.leaderPrevLogTerm, cmd.leaderCommitIndex, cmd.dep_id, cmd.md_cmd, addrChain, state);  // #profile(crpc2) - 3.96%%
  Future::safe_release(f);
}

void FpgaRaftCommo::CrpcAppendEntries3(const parid_t par_id,
              const uint64_t& id,
              const slotid_t slot_id,
              const ballot_t ballot,
              const uint64_t leaderCurrentTerm,
              const uint64_t leaderPrevLogIndex,
              const uint64_t leaderPrevLogTerm,
              const uint64_t leaderCommitIndex,
							const struct DepId dep_id,
              const MarshallDeputy& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const std::vector<AppendEntriesResult>& state){
  Log_info("@@@ FpgaRaft CP 32: FpgaRaftCommo::CrpcAppendEntries3");
  // Log_info("inside FpgaRaftCommo::CrpcAppendEntries; checkpoint 0 @ %d", gettid());
  // auto proxies = rpc_par_proxies_[par_id];
  // FpgaRaftProxy *proxy = nullptr;

  auto proxy = (FpgaRaftProxy *)rpc_proxies_[addrChain[0]];
  auto f = proxy->async_CrpcAppendEntries(id, slot_id, ballot, leaderCurrentTerm, leaderPrevLogIndex, leaderPrevLogTerm, 
                                          leaderCommitIndex, dep_id, cmd, addrChain, state);  // #profile(crpc2) - 3.96%%
  Future::safe_release(f);
}

void FpgaRaftCommo::CrpcAppendEntries_no_chain(const parid_t par_id,
              const uint64_t& id,
              const slotid_t slot_id,
              const ballot_t ballot,
              const uint64_t leaderCurrentTerm,
              const uint64_t leaderPrevLogIndex,
              const uint64_t leaderPrevLogTerm,
              const uint64_t leaderCommitIndex,
							const struct DepId dep_id,
              const MarshallDeputy& cmd, 
              const std::vector<uint16_t>& addrChain, 
              std::vector<AppendEntriesResult>* state,
              const function<void()> &cb){
  Log_info("@@@ FpgaRaft CP 33: FpgaRaftCommo::CrpcAppendEntries_no_chain");
  // Log_info("inside FpgaRaftCommo::CrpcAppendEntries; checkpoint 0 @ %d", gettid());
  // Log_info("$$$ inside FpgaRaftCommo::CrpcAppendEntries_no_chain, calling proxy->CrpcAppendEntries3; tid is %d", gettid());
  auto proxies = rpc_par_proxies_[par_id];
  FpgaRaftProxy *proxy = nullptr;
  FutureAttr fuattr;
  // Log_info("$$$ inside FpgaRaftCommo::CrpcAppendEntries_no_chain; fuattr.callback, current state size 1:%d ; tid is %d",state->size(), gettid());
  fuattr.callback = [this, state, cb] (Future* fu) {
      // // Log_info("*** inside FpgaRaftCommo::BroadcastAppendEntries; received response");
      // Log_info("$$$ inside fuattr.callback, response received; tid is %d", gettid());
      
      vector<AppendEntriesResult> result;			
      fu->get_reply() >> result;
      // fu->get_reply() >> *state;
      // Log_info("$$$ inside FpgaRaftCommo::CrpcAppendEntries_no_chain; fuattr.callback, current state size 2:%d ; tid is %d",state->size(), gettid());
			for (auto r: result){
        // Log_info("$$$ inside FpgaRaftCommo::CrpcAppendEntries_no_chain; fuattr.callback, some result; tid is %d", gettid());
        state->push_back(r);
      }
      // Log_info("$$$ inside FpgaRaftCommo::CrpcAppendEntries_no_chain; fuattr.callback, current state size 2:%d ; tid is %d",state->size(), gettid());
       // Log_info("$$$ inside FpgaRaftCommo::CrpcAppendEntries_no_chain; fuattr.callback checkpoint n; tid is %d", gettid());
       cb();
    };
  proxy = (FpgaRaftProxy *)rpc_proxies_[addrChain[0]];
  auto f = proxy->async_CrpcAppendEntries3(id, slot_id, ballot, leaderCurrentTerm, leaderPrevLogIndex, leaderPrevLogTerm, 
                                          leaderCommitIndex, dep_id, cmd, addrChain, fuattr);  // #profile(crpc2) - 3.96%%
  Future::safe_release(f);

  // Log_info("$$$ returning FpgaRaftCommo::CrpcAppendEntries_no_chain; tid is %d", gettid());
}

} // namespace janus
