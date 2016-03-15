#pragma once

//#include "all.h"
#include "graph.h"
#include "txn-info.h"
#include "marshal-value.h"
#include "command.h"
#include "command_marshaler.h"
//#include "rcc_srpc.h"

/**
 * This is NOT thread safe!!!
 */
namespace rococo {

typedef Vertex<TxnInfo> RccVertex;
typedef vector<RccVertex> RccScc;
class RccGraph {
 public:
//    Graph<PieInfo> pie_gra_;
  Graph <TxnInfo> txn_gra_;

//  std::vector<rrr::Client *> rpc_clients_;
//  std::vector<RococoProxy *> rpc_proxies_;
//  std::vector<std::string> server_addrs_;


  RccGraph() {
    // TODO remove this out, use commo instead.
//    Config::GetConfig()->get_all_site_addr(server_addrs_);
//    rpc_clients_ = std::vector<rrr::Client *>(server_addrs_.size(), nullptr);
//    rpc_proxies_ = std::vector<RococoProxy *>(server_addrs_.size(), nullptr);
  }

  ~RccGraph() {
    // XXX hopefully some memory leak here does not hurt. :(
  }

//  RococoProxy *get_server_proxy(uint32_t id);

  /** on start_req */
  void start_pie(
      txnid_t txn_id,
      RccVertex **tv
  );

  void Aggregate(RccGraph& graph);

  void union_txn_graph(Graph <TxnInfo> &gra) {
    txn_gra_.Aggregate(gra, true);
  }

  std::vector<Vertex < TxnInfo>*> find_txn_scc(TxnInfo &ti) {
    return txn_gra_.FindSCC(ti.id());
  }

  void find_txn_anc_opt(Vertex <TxnInfo> *source,
                        std::unordered_set<Vertex <TxnInfo> *> &ret_set);

  void find_txn_anc_opt(uint64_t txn_id,
                        std::unordered_set<Vertex <TxnInfo> *> &ret_set);

  void find_txn_nearest_anc(Vertex <TxnInfo> *v,
                            std::set<Vertex <TxnInfo> *> &ret_set) {
    for (auto &kv: v->incoming_) {
      ret_set.insert(kv.first);
    }
  }

  void find_txn_scc_nearest_anc(
      Vertex <TxnInfo> *v,
      std::set<Vertex < TxnInfo> *
  > &ret_set
  ) {
    std::vector<Vertex < TxnInfo>*> scc = txn_gra_.FindSCC(v);
    for (auto v: scc) {
      find_txn_nearest_anc(v, ret_set);
    }

    for (auto &scc_v: scc) {
      ret_set.erase(scc_v);
    }

    //if (RandomGenerator::rand(1, 100) == 1) {
    //    Log::info("scc anc size: %d", ret_set.size());
    //}
  }

  int size() const {
    return txn_gra_.size();
  };

  void find_txn_scc_anc_opt(
      uint64_t txn_id,
      std::unordered_set<Vertex < TxnInfo> *
  > &ret_set
  );

  uint64_t MinItfrGraph(
      uint64_t tid,
      RccGraph &gra_m
  );

  void write_to_marshal(rrr::Marshal &m) const;

  void marshal_help_1(rrr::Marshal &m,
                      const std::unordered_set<Vertex<TxnInfo> *> &ret_set,
                      Vertex<TxnInfo> *old_sv) const;

  void marshal_help_2(rrr::Marshal &m,
                      const std::unordered_set<Vertex<TxnInfo> *> &ret_set,
                      Vertex<TxnInfo> *old_sv) const;
};
} // namespace rcc
