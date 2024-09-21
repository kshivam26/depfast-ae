#include "server_worker.h"
#include "service.h"
#include "benchmark_control_rpc.h"
#include "sharding.h"
#include "scheduler.h"
#include "frame.h"
#include "communicator.h"
#include "fpga_raft/coordinator.h"
#include "classic/tpc_command.h"

namespace janus {

void ServerWorker::SetupHeartbeat() {
  bool hb = Config::GetConfig()->do_heart_beat();
  if (!hb) return;
  auto timeout = Config::GetConfig()->get_ctrl_timeout();
  scsi_ = new ServerControlServiceImpl(timeout);
  int n_io_threads = 1;
//  svr_hb_poll_mgr_g = new rrr::PollMgr(n_io_threads);
  svr_hb_poll_mgr_g = svr_poll_mgr_;
//  hb_thread_pool_g = new rrr::ThreadPool(1);
  hb_thread_pool_g = svr_thread_pool_;
  hb_rpc_server_ = new rrr::Server(svr_hb_poll_mgr_g, hb_thread_pool_g);
  hb_rpc_server_->reg(scsi_);

  auto port = this->site_info_->port + ServerWorker::CtrlPortDelta;
  std::string addr_port = std::string("0.0.0.0:") +
      std::to_string(port);
  hb_rpc_server_->start(addr_port.c_str());
  if (hb_rpc_server_ != nullptr) {
    // Log_info("notify ready to control script for %s", bind_addr.c_str());
    scsi_->set_ready();
  }
  Log_info("heartbeat setup for %s on %s",
           this->site_info_->name.c_str(), addr_port.c_str());
}

void ServerWorker::SetupBase() {
  auto config = Config::GetConfig();
  tx_frame_ = Frame::GetFrame(config->tx_proto_);
  tx_frame_->site_info_ = site_info_;

  // this needs to be done before poping table
  sharding_ = tx_frame_->CreateSharding(Config::GetConfig()->sharding_);
  sharding_->BuildTableInfoPtr();

  verify(tx_reg_ == nullptr);
  tx_reg_ = std::make_shared<TxnRegistry>();
  tx_sched_ = tx_frame_->CreateScheduler();
  tx_sched_->txn_reg_ = tx_reg_;
  tx_sched_->SetPartitionId(site_info_->partition_id_);
  tx_sched_->loc_id_ = site_info_->locale_id;
  tx_sched_->site_id_ = site_info_->id;
//  Log_info("initialize site id: %d", (int) site_info_->id);
  sharding_->tx_sched_ = tx_sched_;

  if (config->IsReplicated() &&
      config->replica_proto_ != config->tx_proto_) {
    Log_info("^^^^ inside server_worker; config->replica_proto_: %d", config->replica_proto_);
    rep_frame_ = Frame::GetFrame(config->replica_proto_);
    rep_frame_->site_info_ = site_info_;
    rep_sched_ = rep_frame_->CreateScheduler();
    rep_sched_->txn_reg_ = tx_reg_;
    rep_sched_->loc_id_ = site_info_->locale_id;
    rep_sched_->site_id_ = site_info_->id;
    rep_sched_->tx_sched_ = tx_sched_;
    tx_sched_->rep_frame_ = rep_frame_;
    tx_sched_->rep_sched_ = rep_sched_;
  }
  // add callbacks to execute commands to rep_sched_
  if (rep_sched_ && tx_sched_) {
    rep_sched_->RegLearnerAction(std::bind(&TxLogServer::Next,
                                           tx_sched_,
                                           std::placeholders::_1));
  }
}

void ServerWorker::PopTable() {
  // populate table
  int ret = 0;
  // get all tables
  std::vector<std::string> table_names;

  Log_info("start data population for site %d", site_info_->id);
  ret = sharding_->GetTableNames(site_info_->partition_id_, table_names);
  verify(ret > 0);

  for (auto table_name : table_names) {
    mdb::Schema* schema = new mdb::Schema();
    mdb::symbol_t symbol;
    sharding_->init_schema(table_name, schema, &symbol);
    mdb::Table* tb;

    switch (symbol) {
      case mdb::TBL_SORTED:
        tb = new mdb::SortedTable(table_name, schema);
        break;
      case mdb::TBL_UNSORTED:
        tb = new mdb::UnsortedTable(table_name, schema);
        break;
      case mdb::TBL_SNAPSHOT:
        tb = new mdb::SnapshotTable(table_name, schema);
        break;
      default:
        verify(0);
    }
    tx_sched_->reg_table(table_name, tb);
  }
  verify(sharding_);
  sharding_->PopulateTables(site_info_->partition_id_);
  Log_info("data populated for site: %x, partition: %x",
           site_info_->id, site_info_->partition_id_);
  verify(ret > 0);
}

void ServerWorker::RegisterWorkload() {
  Log_info("^^^^ inside ServerWorker::RegisterWorkload");
  Workload* workload = Workload::CreateWorkload(Config::GetConfig());
  verify(tx_reg_ != nullptr);
  verify(sharding_ != nullptr);
  workload->sss_ = sharding_;
  workload->txn_reg_ = tx_reg_;
  workload->RegisterPrecedures();
}

void ServerWorker::SetupService() {
  // Log_info("enter %s for %s @ %s", __FUNCTION__,
  //          this->site_info_->name.c_str(),
  //          site_info_->GetBindAddress().c_str());

  int ret;
  auto config = Config::GetConfig();
  // set running mode and initialize transaction manager.
  std::string bind_addr = site_info_->GetBindAddress();

  // init rrr::PollMgr 1 threads
  int n_io_threads = 1;
  // Log_info("*** inside ServerWorker::SetupService; value of config->replica_proto_ == MODE_FPGA_RAFT: %d", config->replica_proto_ == MODE_FPGA_RAFT);
  // svr_poll_mgr_ = new rrr::PollMgr(n_io_threads, config->replica_proto_ == MODE_FPGA_RAFT);  // Raft needs a disk thread // ***uncomment for logging
  svr_poll_mgr_ = new rrr::PollMgr(n_io_threads);  
  Reactor::GetReactor()->server_id_ = site_info_->id;
//  svr_thread_pool_ = new rrr::ThreadPool(1);

  // init service implementation

  if (tx_frame_ != nullptr) {
    // Log_info("^^^^ inside ServerWorker::SetupService; cp1");
    services_ = tx_frame_->CreateRpcServices(site_info_->id,
                                             tx_sched_,
                                             svr_poll_mgr_,
                                             scsi_);
    // Log_info("@@@@ number of services1 are: %d", services_.size());
  }

  if (rep_frame_ != nullptr) {
    // Log_info("^^^^ inside ServerWorker::SetupService; cp2");
    auto s2 = rep_frame_->CreateRpcServices(site_info_->id,
                                            rep_sched_,
                                            svr_poll_mgr_,
                                            scsi_);

    services_.insert(services_.end(), s2.begin(), s2.end());
    // Log_info("@@@@ number of services2 are: %d", services_.size());
  }

//  auto& alarm = TimeoutALock::get_alarm_s();
//  ServerWorker::svr_poll_mgr_->add(&alarm);

  uint32_t num_threads = 1;
//  thread_pool_g = new base::ThreadPool(num_threads);

  // init rrr::Server
  rpc_server_ = new rrr::Server(svr_poll_mgr_, svr_thread_pool_);

  // reg services
  for (auto service : services_) {
    rpc_server_->reg(service);
  }

  // start rpc server
  Log_debug("starting server at %s", bind_addr.c_str());
  ret = rpc_server_->start(bind_addr.c_str());
  if (ret != 0) {
    Log_fatal("server launch failed.");
  }

  Log_info("Server %s ready at %s",
           site_info_->name.c_str(),
           bind_addr.c_str());

}

void ServerWorker::WaitForShutdown() {
  Log_debug("%s", __FUNCTION__);
  if (hb_rpc_server_ != nullptr) {
    scsi_->wait_for_shutdown();
    delete hb_rpc_server_;
    delete scsi_;
    if (svr_hb_poll_mgr_g != svr_poll_mgr_)
      svr_hb_poll_mgr_g->release();
    if (hb_thread_pool_g != svr_thread_pool_)
      hb_thread_pool_g->release();

    for (auto service : services_) {
      if (DepTranServiceImpl* s = dynamic_cast<DepTranServiceImpl*>(service)) {
        auto& recorder = s->recorder_;
        if (recorder) {
          auto n_flush_avg_ = recorder->stat_cnt_.peek().avg_;
          auto sz_flush_avg_ = recorder->stat_sz_.peek().avg_;
          Log::info("Log to disk, average log per flush: %lld,"
                        " average size per flush: %lld",
                    n_flush_avg_, sz_flush_avg_);
        }
      }
    }
  }
  Log_debug("exit %s", __FUNCTION__);
}

void ServerWorker::SetupCommo() {
  verify(svr_poll_mgr_ != nullptr);
  // Log_info("*** inside ServerWorker::SetupCommo(); cp 1; tid: %d", gettid());
  if (tx_frame_) {
    // Log_info("*** inside ServerWorker::SetupCommo(); cp 2; tid: %d", gettid());
    tx_commo_ = tx_frame_->CreateCommo(svr_poll_mgr_);
    if (tx_commo_) {
      tx_commo_->loc_id_ = site_info_->locale_id;
    }
    tx_sched_->commo_ = tx_commo_;   // ***uncomment/comment later
    // tx_sched_->commo_  = nullptr;
  }
  if (rep_frame_) {
    // Log_info("*** inside ServerWorker::SetupCommo(); cp 3; tid: %d", gettid());
    rep_commo_ = rep_frame_->CreateCommo(svr_poll_mgr_); // creates connection to every other server
    if (rep_commo_) {
      rep_commo_->loc_id_ = site_info_->locale_id;
    }
    rep_sched_->commo_ = rep_commo_;
    // rep_sched_->commo_ = nullptr;
		rep_sched_->Setup();

    rep_commo_->rep_sched_ = rep_sched_;

    // ks-RM: maybe setup the client service part here
  }
}

void ServerWorker::StartBenchmark(){
  verify(rep_frame_ != nullptr);
  Log_info("############ Inside startBenchmark; cp1");
  // svr_poll_mgr_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>
  auto p_job = (Job*)new OneTimeJob([this](){
    Log_info("*********** going to sleep for sometime");
    // Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
    auto sp_e = Reactor::CreateSpEvent<TimeoutEvent>(500 * 1000);
    sp_e->Wait();
    auto seconds = 60; // TODO: change later
    Log_info("*************** STARTING BENCHMARK; tid: %d", gettid());
    auto beg_time = Time::now();
    auto end_time1 = beg_time + (seconds/3) * pow(10, 6);
    auto end_time2 = beg_time + (seconds*2/3) * pow(10,6);
    auto end_time = beg_time + seconds * pow(10, 6);
    auto max_pending_requests = 200;
    auto batchSize = 40;
    struct timespec start_;
    start_.tv_sec = 0;
    int last_sent = 0;
    int total = 0;
    Counter num_sent_counter;
    Counter num_done_counter;
    std::vector<double> latency_manager;
    bool isTime1Passed = false;
    bool isTime2Passed = false;
    std::string payload(10, 'a');
    auto cnt33 = 0;
    auto cnt66 = 0;
    while(true){
        // Log_info("Inside the loop");
        auto cur_time = Time::now(); // optimize: this call is not scalable.

        int cnt = num_sent_counter.peek_next();
        int num_done = num_done_counter.peek_next();
        if (cur_time > end_time) {
            Log_info("*************** ENDING BENCHMARK; ");
            while (cnt !=num_done){
                Log_info("#### Waiting for all the responses to return; cnt: %d, num_done: %d", cnt, num_done);
                Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                num_done = num_done_counter.peek_next();
            }
            Log_info("throughput: %f and total: %d", (double)(cnt66 - cnt33) / (seconds * 2.0 / 3 - seconds / 3.0), total);
            // Log_info("AVG Throughput: %f", (double)cnt/(seconds));
            ///////////////////////////
            // latency calculation            

            std::sort(latency_manager.begin(), latency_manager.end());
            size_t l_length = latency_manager.size();
            double sum = 0.0;
            int p50 = floor(l_length * 0.5);
            int p90 = floor(l_length * 0.90);
            int p99 = floor(l_length * 0.99);
            int p999 = floor(l_length * 0.999);
            double l50, l90, l99, l999;
            int ii = 0;
            for (; ii < p50; ii++)
                sum += latency_manager[ii];
            l50 = sum / p50;
            for (; ii < p90; ii++)
                sum += latency_manager[ii];
            l90 = sum / p90;
            for (; ii < p99; ii++)
                sum += latency_manager[ii];
            l99 = sum / p99;
            for (; ii < p999; ii++)
                sum += latency_manager[ii];
            l999 = sum / p999;
            Log_info("50.0%% LATENCY: %lf; 90.0%% LATENCY: %lf; 99.0%% LATENCY: %lf; 99.9%% LATENCY: %lf\n", l50 * 1000, l90 * 1000, l99 * 1000, l999 * 1000);

            break;
        }

        if (!isTime1Passed && cur_time > end_time1){
          cnt33 = num_done_counter.peek_next();
          isTime1Passed = true;
        }

        if (!isTime2Passed && cur_time > end_time2){
          cnt66 = num_done_counter.peek_next();
          isTime2Passed = true;
        }

        /*
            actual request generation
        */
        while (true){
            if (cnt-num_done >= max_pending_requests || last_sent >= batchSize){
                // Log_info("pausing the one-time sending job; cnt: %d, num_done:%d", cnt, num_done);
                if (cnt-num_done >= max_pending_requests) {
                  // Log_info("pausing the one-time sending job; cnt: %d, num_done:%d", cnt, num_done);
                  total++;
                }
                Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 2));
                // Log_info("woke up from wait");
                last_sent = 0;
                break;
            }
            // Log_info("Going to send chaining request to all servers");
            cnt = num_sent_counter.next();
            last_sent++;
            // int q = proxies.size()/2+1;
            if (cnt % 20000 == 0) {
                Log_info("%d generated requests", cnt);
            }

            Coroutine::CreateRun([&] () {
              auto coo = rep_frame_->CreateCoordinator(cnt, Config::GetConfig(), 0, nullptr, cnt, nullptr);
              struct timespec last_time;
              clock_gettime(CLOCK_REALTIME, &last_time);
              coo->dep_id_ = 0;
              coo->par_id_ = 0;
              coo->loc_id_ = site_info_->locale_id;
              // Log_info("Inside startBenchmark; cp2");
              auto empty_cmd = std::make_shared<TpcRaftSampleCommand>();
              
              empty_cmd->message_ = payload.c_str();
              // Log_info("Insi32de startBenchmark; cp3");
              auto sp_m = dynamic_pointer_cast<Marshallable>(empty_cmd);
              // Log_info("Inside startBenchmark; cp4");
              ((CoordinatorFpgaRaft *)coo)->Submit(sp_m, std::function<void()>(), std::function<void()>());
              struct timespec t_buf;
              clock_gettime(CLOCK_REALTIME, &t_buf);
              latency_manager.push_back(t_buf.tv_sec - last_time.tv_sec + (t_buf.tv_nsec - last_time.tv_nsec) / 1000000000.0);
              // Log_info("Inside startBenchmark; cp4");
              num_done_counter.next();
            });
        }

    }
  });
  Log_info("############ Now adding job to poll_mgr_");
  shared_ptr<Job> sp_job(p_job);
  svr_poll_mgr_->add(sp_job);
  Log_info("############ Added job to poll_mgr_");
}


void ServerWorker::Pause() {
  rep_sched_->Pause();
  svr_poll_mgr_->pause();
}

void ServerWorker::Resume() {
  svr_poll_mgr_->resume();
  rep_sched_->Resume();
}

void ServerWorker::ShutDown() {
  Log_debug("deleting services, num: %d", services_.size());
  delete rpc_server_;
  for (auto service : services_) {
    delete service;
  }
  delete rep_frame_;
//  thread_pool_g->release();
  svr_poll_mgr_->release();
}
int ServerWorker::DbChecksum() {
  auto cs = this->tx_sched_->mdb_txn_mgr_->Checksum();
  Log_info("site_id: %d shard_id: %d checksum: %x", (int)this->site_info_->id,
           (int)this->site_info_->partition_id_, (int) cs);
  return cs;
}
} // namespace janus
