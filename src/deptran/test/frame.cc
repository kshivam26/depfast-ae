#include "../__dep__.h"
#include "../constants.h"
#include "frame.h"
#include "coordinator.h"
#include "server.h"
#include "service.h"
#include "commo.h"

namespace janus {

REG_FRAME(MODE_TEST, vector<string>({"test"}), TestFrame);

TestFrame::TestFrame(int mode) : Frame(mode) {}

TestFrame::~TestFrame() { 
  // Log_info("Inside CreateScheduler");
}


Coordinator *TestFrame::CreateCoordinator(cooid_t coo_id,
                                          Config *config,
                                          int benchmark,
                                          ClientControlServiceImpl *ccsi,
                                          uint32_t id,
                                          shared_ptr<TxnRegistry> txn_reg) {
  verify(config != nullptr);
  CoordinatorTest *coo;
  coo = new CoordinatorTest(coo_id, benchmark, ccsi, id);
  coo->frame_ = this;
  verify(commo_ != nullptr);
  coo->commo_ = commo_;
  verify(sch_ != nullptr);
  coo->sch_ = this->sch_;
  coo->slot_hint_ = &slot_hint_;
  coo->slot_id_ = slot_hint_++;
  coo->n_replica_ = config->GetPartitionSize(site_info_->partition_id_);
  coo->loc_id_ = this->site_info_->locale_id;
  verify(coo->n_replica_ != 0);
  // Log_info("create new test coord, coo_id: %d", (int) coo->coo_id_);
  return coo;
}


TxLogServer *TestFrame::CreateScheduler() {
  // Log_info("Inside CreateScheduler");
  if (sch_ == nullptr) {
    sch_ = new TestServer(this);
  } else {
    verify(0);
  }
  // Log_info("create test sched loc: %d", this->site_info_->locale_id);
  return sch_;
}

Communicator *TestFrame::CreateCommo(PollMgr *poll) {
  // Log_info("Inside CreateCommo");
  if (commo_ == nullptr) {
    commo_ = new TestCommo(poll);
  }
  return commo_;
}

vector<rrr::Service *> TestFrame::CreateRpcServices(uint32_t site_id,
                                                    TxLogServer *rep_sched,
                                                    rrr::PollMgr *poll_mgr,
                                                    ServerControlServiceImpl *scsi) {
  // Log_info("Inside CreateRpcServices");
  auto config = Config::GetConfig();
  auto result = std::vector<Service *>();
  switch (config->replica_proto_) {
    case MODE_TEST:result.push_back(new TestServiceImpl(rep_sched));
    default:break;
  }
  return result;
}

}

