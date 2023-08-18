#include "../__dep__.h"
#include "../constants.h"
#include "frame.h"
// #include "coordinator.h"
#include "server.h"
#include "service.h"
#include "commo.h"
#include "config.h"

namespace janus {

REG_FRAME(MODE_SAMPLE_CRPC, vector<string>({"sample_crpc"}), SampleCrpcFrame);

SampleCrpcFrame::SampleCrpcFrame(int mode) : Frame(mode) {

}

// Coordinator *SampleCrpcFrame::CreateCoordinator(cooid_t coo_id,
//                                                 Config *config,
//                                                 int benchmark,
//                                                 ClientControlServiceImpl *ccsi,
//                                                 uint32_t id,
//                                                 shared_ptr<TxnRegistry> txn_reg) {
//   // Log_info("*** Inside Coordinator *SampleCrpcFrame::CreateCoordinator");
//   verify(config != nullptr);
//   CoordinatorSampleCrpc *coo;
//   coo = new CoordinatorSampleCrpc(coo_id,
//                                   benchmark,
//                                   ccsi,
//                                   id);
//   coo->frame_ = this;
//   verify(commo_ != nullptr);
//   coo->commo_ = commo_;
//   /* TODO: remove when have a class for common data */
//   verify(sch_ != nullptr);
//   coo->sch_ = this->sch_;
//   coo->slot_hint_ = &slot_hint_;
//   coo->slot_id_ = slot_hint_++;
//   coo->n_replica_ = config->GetPartitionSize(site_info_->partition_id_);
//   coo->loc_id_ = this->site_info_->locale_id;
//   verify(coo->n_replica_ != 0); // TODO
//   Log_debug("create new fpga raft coord, coo_id: %d", (int) coo->coo_id_);
//   return coo;
// }

TxLogServer *SampleCrpcFrame::CreateScheduler() {
  if(sch_ == nullptr)
  {
    sch_ = new SampleCrpcServer(this);
  }
  else
  {
    verify(0) ;
  }
  Log_debug("create new fpga raft sched loc: %d", this->site_info_->locale_id);
  return sch_ ;
}

Communicator *SampleCrpcFrame::CreateCommo(PollMgr *poll) {
  // We only have 1 instance of SampleCrpcFrame object that is returned from
  // GetFrame method. SampleCrpcCommo currently seems ok to share among the
  // clients of this method.
  if (commo_ == nullptr) {
    commo_ = new SampleCrpcCommo(poll);
  }
  return commo_;
}

vector<rrr::Service *>
SampleCrpcFrame::CreateRpcServices(uint32_t site_id,
                                   TxLogServer *rep_sched,
                                   rrr::PollMgr *poll_mgr,
                                   ServerControlServiceImpl *scsi) {
  auto config = Config::GetConfig();
  auto result = std::vector<Service *>();
  switch (config->replica_proto_) {
    case MODE_FPGA_RAFT:result.push_back(new SampleCrpcServiceImpl(rep_sched));
    default:break;
  }
  return result;
}

} // namespace janus;
