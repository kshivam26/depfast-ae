#include "../__dep__.h"
#include "../constants.h"
#include "commo.h"
#include "server.h"

namespace janus {

TestCommo::TestCommo(PollMgr* poll) : Communicator(poll) {
//  verify(poll != nullptr);
}

void TestCommo::cRPC(const parid_t par_id,
                         const uint64_t& id,
                         const MarshallDeputy& cmd, 
                         const std::vector<uint16_t>& addrChain, 
                         const MarshallDeputy& state) {
  // Log_info("*** inside TestCommo::cRPC");

  FutureAttr fuattr;
  fuattr.callback = [](Future *fu) {
    // Log_info("*** received a response back, in cRPC");
  };

  auto proxies = rpc_par_proxies_[par_id];
  TestProxy *proxy = nullptr;

  proxy = (TestProxy *)rpc_proxies_[addrChain[0]];
  // for (auto p : proxies)          // #cPRC TODO: room for optimization?
  // {
  //   if (p.first == addrChain[0])
  //   {
  //       proxy = (TestProxy *)p.second;
  //       break;
  //   }
  // }

  // Log_info("*** calling proxy->proxy->async_cRPC");
  auto f = proxy->async_cRPC(id, cmd, addrChain, state, fuattr);  // #profile - 1.57%
  Future::safe_release(f);
}

}
