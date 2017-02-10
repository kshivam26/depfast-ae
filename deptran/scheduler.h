#pragma once
#include "__dep__.h"
#include "constants.h"
#include "command.h"
#include "epochs.h"
namespace rococo {

class DTxn;
class TxnRegistry;
class Executor;
class Coordinator;
class Frame;
class Communicator;
class RococoCommunicator;
class Scheduler {
 public:
  locid_t loc_id_ = -1;
  siteid_t site_id_ = -1;
  unordered_map<txnid_t, DTxn *> dtxns_ = {};
  unordered_map<txnid_t, mdb::Txn *> mdb_txns_ = {};
  unordered_map<txnid_t, Executor*> executors_ = {};
  function<void(ContainerCommand&)> learner_action_ =
      [] (ContainerCommand&) -> void {verify(0);};

  mdb::TxnMgr *mdb_txn_mgr_;
  int mode_;
  Recorder *recorder_ = nullptr;
  Frame *frame_ = nullptr;
  Frame *rep_frame_ = nullptr;
  Scheduler* rep_sched_ = nullptr;
  Communicator* commo_{nullptr};
  //  Coordinator* rep_coord_ = nullptr;
  TxnRegistry* txn_reg_ = nullptr;
  parid_t partition_id_{};
  std::recursive_mutex mtx_{};

  bool epoch_enabled_{false};
  EpochMgr epoch_mgr_{};
  std::time_t last_upgrade_time_{0};
  map<parid_t, map<siteid_t, epoch_t>> epoch_replies_{};
  bool in_upgrade_epoch_{false};
  const int EPOCH_DURATION = 5;

#ifdef CHECK_ISO
  typedef map<Row*, map<column_id_t, int>> deltas_t;
  deltas_t deltas_{};

  void MergeDeltas(deltas_t deltas) {
    verify(deltas.size() > 0);
    for (auto& pair1: deltas) {
      Row* r = pair1.first;
      for (auto& pair2: pair1.second) {
        column_id_t c = pair2.first;
        int delta = pair2.second;
        deltas_[r][c] += delta;
        int v = r->get_column(c).get_i32();
        int x = deltas_[r][c];
      }
    }
    deltas.clear();
  }

  void CheckDeltas() {
    for (auto& pair1: deltas_) {
      Row* r = pair1.first;
      for (auto& pair2: pair1.second) {
        column_id_t c = pair2.first;
        int delta = pair2.second;
        int v = r->get_column(c).get_i32();
        verify(delta == v);
      }
    }
  }
#endif

  RococoCommunicator* commo() {
    verify(commo_ != nullptr);
    return (RococoCommunicator*) commo_;
  }

  Scheduler();
  Scheduler(int mode);
  virtual ~Scheduler();

  virtual void SetPartitionId(parid_t par_id) {
    partition_id_ = par_id;
  }

  Coordinator*CreateRepCoord();
  virtual DTxn *GetDTxn(txnid_t tid);
  virtual DTxn *CreateDTxn(txnid_t tid, bool ro = false);
  virtual DTxn *GetOrCreateDTxn(txnid_t tid, bool ro = false);
  virtual DTxn *CreateDTxn(epoch_t epoch, txnid_t txn_id, bool read_only = false);
  virtual DTxn *GetOrCreateDTxn(epoch_t epoch, txnid_t txn_id);
  void DestroyDTxn(i64 tid);

  Executor* GetExecutor(txnid_t txn_id);
  Executor* CreateExecutor(txnid_t txn_id);
  Executor* GetOrCreateExecutor(txnid_t txn_id);
  virtual void DestroyExecutor(txnid_t txn_id);
  virtual void TrashExecutor(txnid_t txn_id);

  inline int get_mode() { return mode_; }

  // Below are function calls that go deeper into the mdb.
  // They are merged from the called TxnRunner.

  inline mdb::Table
  *get_table(const string &name) {
    return mdb_txn_mgr_->get_table(name);
  }

  virtual mdb::Txn* GetMTxn(const i64 tid);
  virtual mdb::Txn *GetOrCreateMTxn(const i64 tid);
  virtual mdb::Txn *RemoveMTxn(const i64 tid);

  void get_prepare_log(i64 txn_id,
                       const std::vector<i32> &sids,
                       std::string *str
  );


  // TODO: (Shuai: I am not sure this is supposed to be here.)
  // I think it used to initialized the database?
  // So it should be somewhere else?
  void reg_table(const string &name,
                 mdb::Table *tbl
  );

  void RegLearnerAction(function<void(ContainerCommand&)> learner_action) {
    learner_action_ = learner_action;
  }

  virtual void OnLearn(ContainerCommand& cmd) {verify(0);};

  // epoch related functions
  void TriggerUpgradeEpoch();
  void UpgradeEpochAck(parid_t par_id, siteid_t site_id, int res);
  virtual int32_t OnUpgradeEpoch(uint32_t old_epoch);
  virtual void OnTruncateEpoch(uint32_t old_epoch);
};


} // namespace rococo
