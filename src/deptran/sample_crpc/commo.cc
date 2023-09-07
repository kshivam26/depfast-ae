
#include "commo.h"
#include "../rcc/graph.h"
#include "../rcc/graph_marshaler.h"
#include "../command.h"
#include "../procedure.h"
#include "../command_marshaler.h"
#include "../rcc_rpc.h"

namespace janus {

SampleCrpcCommo::SampleCrpcCommo(PollMgr* poll) : Communicator(poll) {
//  verify(poll != nullptr);
}

void SampleCrpcCommo::CrpcAppendEntries3(const parid_t par_id,
              const uint64_t& id,
              const int64_t& value1,
              const int64_t& value2, 
              const std::vector<uint16_t>& addrChain, 
              const std::vector<ResultAdd>& state){
  Log_info("inside SampleCrpcCommo::CrpcAppendEntries; checkpoint 0 @ %d", gettid());
  // auto proxies = rpc_par_proxies_[par_id];
  // SampleCrpcProxy *proxy = nullptr;

  auto proxy = (SampleCrpcProxy *)rpc_proxies_[addrChain[0]];
  auto f = proxy->async_CrpcAppendEntries(id, value1, value2, addrChain, state);  // #profile(crpc2) - 3.96%%
  Future::safe_release(f);
}

shared_ptr<SampleCrpcAppendQuorumEvent> SampleCrpcCommo::crpc_add(parid_t par_id,
                                      siteid_t leader_site_id,
                                      const int64_t& value1,
                                      const int64_t& value2,
                                      shared_ptr<Marshallable> cmd) {
  Log_info("Inside SampleCrpcCommo::crpc_add");
  static bool hasPrinted = false;  // Static variable to track if it has printed
  if (!hasPrinted) {
      Log_info("In crpcAppendEntries_ring_back; tid of leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }

  int n = Config::GetConfig()->GetPartitionSize(par_id);
  auto e = Reactor::CreateSpEvent<SampleCrpcAppendQuorumEvent>(n, n/2 + 1);
  auto proxies = rpc_par_proxies_[par_id];

  unordered_set<std::string> ip_addrs {};
  std::vector<std::shared_ptr<rrr::Client>> clients;

  WAN_WAIT;
  std::vector<uint16_t> sitesInfo_; // additional; looks like can be computed in cRPC call

  for (auto& p : proxies) {
    auto id = p.first;
    auto proxy = (SampleCrpcProxy*) p.second;
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
    auto proxy = (SampleCrpcProxy*) p.second;
    auto cli_it = rpc_clients_.find(follower_id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();
    }
	  if (p.first == leader_site_id) {
        // fix the 1c1s1p bug
        // Log_info("leader_site_id %d", leader_site_id);
        e->FeedResponse(true, 1, ip);
        continue;
    }

    MarshallDeputy md(cmd);
		verify(md.sp_data_ != nullptr);
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

    std::vector<ResultAdd> state;

    // **** uncomment/comment the fuattr; just testing if ringback is even more costly
    // FutureAttr fuattr;

    // fuattr.callback = [this, e, isLeader, currentTerm, follower_id, n, ip] (Future* fu) {
    //   Log_info("*** inside fuattr.callback, response received; tid is %d", gettid());
    // };
    // just call cRPC something with the above paramters, and no other changes
    
    Log_info("*** SampleCrpcCommo::crpc_add auto f = proxy->");
    
    auto f = proxy->async_CrpcAppendEntries(crpc_id, 
                                                        value1,
                                                        value2,
                                                        sitesInfo_, state); // this can definitely be pushed into the cRPC function below // #profile (crpc2) - 2.05%
    Future::safe_release(f);

    // this too should be abstracted
    cRPCEvents[crpc_id] = e;

    // rather than breaking, do something else; when iterating through proxies
    break;

    }
  // // Log_info("*** returning from SampleCrpcCommo::crpc_ring_BroadcastAppendEntries");
  return e;
}

shared_ptr<SampleCrpcAppendQuorumEvent> SampleCrpcCommo::broadcast_add(parid_t par_id,
                                      siteid_t leader_site_id,
                                      const int64_t& value1,
                                      const int64_t& value2,
                                      shared_ptr<Marshallable> cmd) {
  static bool hasPrinted = false;  // Static variable to track if it has printed

  if (!hasPrinted) {
      Log_info("in no cRPC; tid of leader is %d", gettid());
      hasPrinted = true;  // Update the static variable
  }
  static uint64_t count = 0;
  count++;
  // Log_info("*** inside void SampleCrpcCommo::BroadcastAppendEntries; count: %ld", count);
  // Log_info("*** inside void SampleCrpcCommo::BroadcastAppendEntries; slot_id: %ld; tid is: %d", slot_id, gettid());

  int n = Config::GetConfig()->GetPartitionSize(par_id);
  auto e = Reactor::CreateSpEvent<SampleCrpcAppendQuorumEvent>(n, n/2 + 1);
  auto proxies = rpc_par_proxies_[par_id];

  unordered_set<std::string> ip_addrs {};
  std::vector<std::shared_ptr<rrr::Client>> clients;

  vector<Future*> fus;
  WAN_WAIT;

  for (auto& p : proxies) {
    auto id = p.first;
    auto proxy = (SampleCrpcProxy*) p.second;
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
    auto proxy = (SampleCrpcProxy*) p.second;
    auto cli_it = rpc_clients_.find(follower_id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();  
    }
	if (p.first == leader_site_id) {
        // fix the 1c1s1p bug
        // Log_info("leader_site_id %d", leader_site_id);
        
        e->FeedResponse(true, 1, ip);
        continue;
    }
    FutureAttr fuattr;
    struct timespec begin;
    clock_gettime(CLOCK_MONOTONIC, &begin);

    fuattr.callback = [this, e, n, ip, begin] (Future* fu) {
      Log_info("$$$ inside fuattr.callback, response received; count: %ld", count);
      Log_info("*** inside SampleCrpcCommo::BroadcastAppendEntries; received response");
      int64_t accept = std::numeric_limits<int64_t>::min();
			
			fu->get_reply() >> accept;
			
			struct timespec end;
			//clock_gettime(CLOCK_MONOTONIC, &begin);
			this->outbound--;
			Log_info("reply from server: %s and is_ready: %d", ip.c_str(), e->IsReady());
			clock_gettime(CLOCK_MONOTONIC, &end);
			//Log_info("time of reply on server %d: %ld", follower_id, (end.tv_sec - begin.tv_sec)*1000000000 + end.tv_nsec - begin.tv_nsec);
			
      bool y = (accept != std::numeric_limits<int64_t>::min());
      e->FeedResponse(y, 1, ip);
    };
    MarshallDeputy md(cmd);
		verify(md.sp_data_ != nullptr);
		outbound++;
    
    // // Log_info("*** inside SampleCrpcCommo::BroadcastAppendEntries; calling proxy->async_AppendEntries");
    auto f = proxy->async_BroadcastAppendEntries(value1,
                                        value2,
                                        fuattr); // #profile - 1.36%
    Future::safe_release(f);
  }
  verify(!e->IsReady());
  // // Log_info("*** returning from SampleCrpcCommo::BroadcastAppendEntries");
  return e;
}

} // namespace janus