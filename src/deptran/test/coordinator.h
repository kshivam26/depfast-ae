/*
#pragma once

#include "../__dep__.h"
#include "../coordinator.h"
#include "../frame.h"

namespace janus {

class TestCommo;
class TestServer;
class CoordinatorTest : public Coordinator {
  public:
    FpgaRaftServer* sch_ = nullptr;
    CoordinatorTest(uint32_t coo_id,
  					int32_t benchmark,
                    ClientControlServiceImpl *ccsi,
                    uint32_t thread_id);
    virtual ~CoordinatorTest();
  private:
    TestCommo *commo() {
      verify(commo_);
      return (TestCommo *)commo_;
    }
};

}
*/
