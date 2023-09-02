#include "../__dep__.h"
#include "../constants.h"
#include "commo.h"
#include "server.h"

namespace janus {

TestCommo::TestCommo(PollMgr* poll) : Communicator(poll) {
//  verify(poll != nullptr);
}

shared_ptr<ChainQuorumEvent> TestCommo::cRPC(parid_t par_id, siteid_t leader_site_id, shared_ptr<Marshallable> cmd) {
  static bool hasPrinted = false;  // Static variable to track if it has printed
  if (!hasPrinted) {
    Log_info("In TestCommo::cRPC; tid of leader is %d", gettid());
    hasPrinted = true;  // Update the static variable
  }

  int n = Config::GetConfig()->GetPartitionSize(par_id);
  auto e = Reactor::CreateSpEvent<ChainQuorumEvent>(n, n/2 + 1);
  auto proxies = rpc_par_proxies_[par_id];

  unordered_set<std::string> ip_addrs {};
  std::vector<std::shared_ptr<rrr::Client>> clients;

  WAN_WAIT;
  std::vector<uint16_t> sitesInfo_; // additional; looks like can be computed in cRPC call

  for (auto& p : proxies) {
    auto id = p.first;
    auto proxy = (TestProxy*) p.second;
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
    auto proxy = (TestProxy*) p.second;
    auto cli_it = rpc_clients_.find(follower_id);
    std::string ip = "";
    if (cli_it != rpc_clients_.end()) {
      ip = cli_it->second->host();
    }
    if (p.first == leader_site_id) {
      // fix the 1c1s1p bug
      // Log_info("leader_site_id %d", leader_site_id);
      // e->FeedResponse(true, prevLogIndex + 1, ip);
      continue;
    }

    MarshallDeputy md(cmd);
    verify(md.sp_data_ != nullptr);
    outbound++;
    DepId di;
    // di.str = "dep";
    // di.id = dep_id;

    // auto ae_cmd = std::make_shared<AppendEntriesCommand>(slot_id, 
    //                                                      ballot, 
    //                                                      currentTerm, 
    //                                                      prevLogIndex, 
    //                                                      prevLogTerm, 
    //                                                      commitIndex, 
    //                                                      di, 
    //                                                      md); // call missing just fuattr parameter, everything else same

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
    auto f = proxy->async_cRPCSVC(crpc_id, sitesInfo_, md); // this can definitely be pushed into the cRPC function below // #profile (crpc2) - 2.05%
    Future::safe_release(f);

    // this too should be abstracted
    cRPCEvents[crpc_id] = e;

    // rather than breaking, do something else; when iterating through proxies
    break;

    }
  verify(!e->IsReady());
  Log_info("*** returning from TestCommo::cRPC");
  return e;
}

void TestCommo::cRPC2(const uint64_t& id, const std::vector<uint16_t>& addrChain, const MarshallDeputy& cmd) {
  auto proxy = (TestProxy *)rpc_proxies_[addrChain[0]];
  auto f = proxy->async_cRPCSVC(id, addrChain, cmd);
  Future::safe_release(f);
}

}
