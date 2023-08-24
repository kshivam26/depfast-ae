#pragma once

#include "../__dep__.h"
#include "../coordinator.h"
#include "../frame.h"

namespace janus {

class SampleCrpcCommo;
class SampleCrpcServer;
class CoordinatorSampleCrpc : public Coordinator {
 public:
  SampleCrpcServer* sch_ = nullptr;
 private:
    SampleCrpcCommo *commo() {
    // TODO fix this.
    verify(commo_ != nullptr);
    return (SampleCrpcCommo *) commo_;
  }
 public:
  shared_ptr<Marshallable> cmd_{nullptr};
  CoordinatorSampleCrpc(uint32_t coo_id,
                        int32_t benchmark,
                        ClientControlServiceImpl *ccsi,
                        uint32_t thread_id);

  void DoTxAsync(TxRequest &req) override {Log_info("inside CoordinatorSampleCrpc::DoTxAsync in header file");}
  void Submit(shared_ptr<Marshallable> &cmd,
              const std::function<void()> &func = []() {},
              const std::function<void()> &exe_callback = []() {}) override;

  void AppendEntries();

  void Reset() override {}
  void Restart() override { verify(0); }
};

} //namespace janus