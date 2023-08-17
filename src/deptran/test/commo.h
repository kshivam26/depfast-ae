#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../communicator.h"
#include "server.h"

namespace janus {

class TestCommo : public Communicator {
friend class TestProxy;
  public:
    TestCommo() = delete;
    TestCommo(PollMgr*);
    void cRPC(const parid_t par_id,
              const uint64_t& id,
              const MarshallDeputy& cmd, 
              const std::vector<uint16_t>& addrChain, 
              const MarshallDeputy& state);
};

}
