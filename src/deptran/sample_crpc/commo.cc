
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

void SampleCrpcCommo::cRPC(const parid_t par_id,
              const uint64_t& id,
              const MarshallDeputy& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const MarshallDeputy& state){
  // // Log_info("*** inside SampleCrpcCommo::cRPC");

  FutureAttr fuattr;
  fuattr.callback = [](Future *fu)
  {
    // // Log_info("*** received a response back, in cRPC");
  };

  auto proxies = rpc_par_proxies_[par_id];
  SampleCrpcProxy *proxy = nullptr;

  proxy = (SampleCrpcProxy *)rpc_proxies_[addrChain[0]];
  // for (auto p : proxies)          // #cPRC TODO: room for optimization?
  // {
  //   if (p.first == addrChain[0])
  //   {
  //       proxy = (SampleCrpcProxy *)p.second;
  //       break;
  //   }
  // }

  // // Log_info("*** calling proxy->proxy->async_cRPC");
  auto f = proxy->async_cRPC(id, cmd, addrChain, state, fuattr);  // #profile - 1.57%
  Future::safe_release(f);
}


} // namespace janus