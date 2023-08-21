#pragma once

#include "../__dep__.h"
#include "../coordinator.h"
#include "../frame.h"

namespace janus {

class TestCommo;
class TestServer;
class CoordinatorTest : public Coordinator {
  public:
    TestServer* sch_ = nullptr;
    CoordinatorTest(uint32_t coo_id,
  					int32_t benchmark,
                    ClientControlServiceImpl *ccsi,
                    uint32_t thread_id);
    virtual ~CoordinatorTest();
    void DoTxAsync(TxRequest &req) override {Log_info("inside CoordinatorTest::DoTxAsync in header file");}
    void Submit(shared_ptr<Marshallable> &cmd,
              const std::function<void()> &func = []() {},
              const std::function<void()> &exe_callback = []() {}) override {};
    void Restart() override { verify(0); }
  private:
    TestCommo *commo() {
      verify(commo_);
      return (TestCommo *)commo_;
    }
};

}
