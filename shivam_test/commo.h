#include "../src/rrr/reactor/quorum_event.h"

class ExampleQuorumEvent: public janus::QuorumEvent {
 public:
  using janus::QuorumEvent::QuorumEvent;
//  ballot_t max_ballot_{0};
  bool HasAcceptedValue() {
    // TODO implement this
    return false;
  }
  void FeedResponse(bool y) {
    if (y) {
      printf("feeding yes in the response");
      VoteYes();
    } else {
      VoteNo();
    }
  }
};

// shared_ptr<GetLeaderQuorumEvent> Communicator::BroadcastGetLeader(
//     parid_t par_id, locid_t cur_pause) {
//   int n = Config::GetConfig()->GetPartitionSize(par_id);
//   auto e = Reactor::CreateSpEvent<GetLeaderQuorumEvent>(n - 1, 1);
//   auto proxies = rpc_par_proxies_[par_id];
//   WAN_WAIT;
//   for (auto& p : proxies) {
//     if (p.first == cur_pause) continue;
//     auto proxy = p.second;
//     FutureAttr fuattr;
//     fuattr.callback = [e, p](Future* fu) {
//       bool_t is_leader = false;
//       fu->get_reply() >> is_leader;
//       e->FeedResponse(is_leader, p.first);
//     };
//     Future::safe_release(proxy->async_IsFPGALeader(par_id, fuattr));
//   }
//   return e;
// }