
#include "commo.h"
#include "../rcc/graph.h"
#include "../rcc/graph_marshaler.h"
#include "../command.h"
#include "../procedure.h"
#include "../command_marshaler.h"
#include "../rcc_rpc.h"
#include "append_entries_command.h"

namespace janus {

SampleCrpcCommo::SampleCrpcCommo(PollMgr* poll) : Communicator(poll) {
//  verify(poll != nullptr);
}

void SampleCrpcCommo::CrpcAppendEntries(const parid_t par_id,
              const uint64_t& id,
              const AppendEntriesCommandToyCrpc& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const std::vector<AppendEntriesAdd>& state){
  // Log_info("inside SampleCrpcCommo::CrpcAppendEntries; checkpoint 0 @ %d", gettid());
  auto proxies = rpc_par_proxies_[par_id];
  SampleCrpcProxy *proxy = nullptr;

  proxy = (SampleCrpcProxy *)rpc_proxies_[addrChain[0]];
  auto f = proxy->async_CrpcAppendEntries(id, cmd.md_cmd, addrChain, state);  // #profile(crpc2) - 3.96%%
  Future::safe_release(f);
}

} // namespace janus