#include "demo.h"

// class ExampleQuorumEvent: public janus::QuorumEvent {
//  public:
//   using janus::QuorumEvent::QuorumEvent;
// //  ballot_t max_ballot_{0};
//   bool HasAcceptedValue() {
//     // TODO implement this
//     return false;
//   }
//   void FeedResponse(bool y) {
//     if (y) {
//       printf("feeding yes in the response");
//       VoteYes();
//     } else {
//       VoteNo();
//     }
//   }
// };

namespace janus {
  class Communicator {
    public:
      std::map<std::string, janus::DemoProxy *> proxies_;
      Communicator(std::vector<std::string> serverAddr);
  };
} // namespace demo