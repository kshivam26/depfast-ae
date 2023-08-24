
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

void SampleCrpcCommo::CrpcAppendEntries(const parid_t par_id,
              const uint64_t& id,
              const int64_t& value1,
              const int64_t& value2, 
              const std::vector<uint16_t>& addrChain, 
              const std::vector<ResultAdd>& state){
  Log_info("inside SampleCrpcCommo::CrpcAppendEntries; checkpoint 0 @ %d", gettid());
  auto proxies = rpc_par_proxies_[par_id];
  SampleCrpcProxy *proxy = nullptr;

  proxy = (SampleCrpcProxy *)rpc_proxies_[addrChain[0]];
  auto f = proxy->async_CrpcAppendEntries(id, value1, value2, addrChain, state);  // #profile(crpc2) - 3.96%%
  Future::safe_release(f);
}

void SampleCrpcCommo::crpc_add(parid_t par_id,
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
    sitesInfo_.push_back(id); // #cPRC additional
		//clients.push_back(cli);
  }
  //e->clients_ = clients;

  // TODO: remove line below, adding this to shorten the chain
  // sitesInfo_.pop_back();

  // MarshallDeputy aes_md(dynamic_pointer_cast<Marshallable>(std::make_shared<AppendEntriesCommandState>())); // additional
  for (auto& p : proxies) {    
    auto follower_id = p.first;
    auto proxy = (SampleCrpcProxy*) p.second;
    auto cli_it = rpc_clients_.find(follower_id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();
    }

    MarshallDeputy md(cmd); // commented, cause segmentation fault?
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
    uint64_t crpc_id;
    // // Log_info("*** crpc_id is: %d", crpc_id); // verify it's never the same
    // verify(cRPCEvents.find(crpc_id) == cRPCEvents.end());

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

    // rather than breaking, do something else; when iterating through proxies
    break;

    }
  // // Log_info("*** returning from SampleCrpcCommo::crpc_ring_BroadcastAppendEntries");
}

} // namespace janus