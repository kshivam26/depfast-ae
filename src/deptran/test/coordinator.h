#pragma once

#include "../__dep__.h"
#include "../coordinator.h"
#include "../frame.h"

namespace janus {

class TestCommo;
class TestServer;
class CoordinatorTest : public Coordinator {
  public:
    shared_ptr<Marshallable> cmd_{nullptr};
    TestServer* sch_ = nullptr;
    uint32_t n_replica_ = 0;
    slotid_t slot_id_ = 0;
    slotid_t *slot_hint_ = nullptr;

    uint32_t n_replica() {
      verify(n_replica_ > 0);
      return n_replica_;
    }
    CoordinatorTest(uint32_t coo_id,
                    int32_t benchmark,
                    ClientControlServiceImpl *ccsi,
                    uint32_t thread_id);
    virtual ~CoordinatorTest();
    void DoTxAsync(TxRequest &req) override {Log_info("inside CoordinatorTest::DoTxAsync in header file");}
    void Submit(shared_ptr<Marshallable> &cmd,
              const std::function<void()> &func = []() {},
              const std::function<void()> &exe_callback = []() {}) override;
    void Restart() override { verify(0); }
    void StartChain();
  private:
    bool in_submission = false; // debug
    bool in_start_chain = false; // debug
    uint64_t minIndex = 0;
    TestCommo *commo() {
      verify(commo_);
      return (TestCommo *)commo_;
    }
};

}
