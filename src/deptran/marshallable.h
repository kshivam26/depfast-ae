#pragma once
#include "__dep__.h"

namespace janus {

class Marshallable {
 public:
  int32_t kind_{0};
//  int32_t __debug_{10};
  Marshallable() = delete;
  explicit Marshallable(int32_t k): kind_(k) {};
  virtual ~Marshallable() {
//    if (__debug_ != 10) {
//      verify(0);
//    }
//    __debug_ = 30;
//    // Log_debug("destruct marshallable.");
  };
  virtual Marshal& ToMarshal(Marshal& m) const;
  virtual Marshal& FromMarshal(Marshal& m);
};

class MarshallDeputy {
 public:
  typedef unordered_map<int32_t, function<Marshallable*()>> MarContainer;
  static MarContainer& Initializers();
  static int RegInitializer(int32_t, function<Marshallable*()>);
  static function<Marshallable*()> GetInitializer(int32_t);

 public:
  shared_ptr<Marshallable> sp_data_{nullptr};
  int32_t kind_{0};
  enum Kind {
    UNKNOWN = 0,
    EMPTY_GRAPH = 1,
    RCC_GRAPH = 2,
    CONTAINER_CMD = 3,
    CMD_TPC_PREPARE = 4,
    CMD_TPC_COMMIT = 5,
    CMD_VEC_PIECE = 6,
    CMD_TPC_EMPTY = 7,
    CMD_TPC_PREPARE_CAROUSEL = 8,
		CMD_BLK_PXS = 9,
    CMD_NOOP = 10,
    CMD_TPC_BATCH = 11,
    CMD_RAFT_APPEND_ENTRIES = 12, // cRPC appendEntries
    CMD_RAFT_APPEND_ENTRIES_STATE = 13 // cRPC appendEntries
  };
  /**
   * This should be called by the rpc layer.
   */
  MarshallDeputy() : kind_(UNKNOWN){}
  /**
   * This should be called by inherited class as instructor.
   * @param kind
   */
  explicit MarshallDeputy(shared_ptr<Marshallable> m): sp_data_(std::move(m)) {
    kind_ = sp_data_->kind_;
  }

  Marshal& CreateActualObjectFrom(Marshal& m);
  void SetMarshallable(shared_ptr<Marshallable> m) {
    verify(sp_data_ == nullptr);
    sp_data_ = m;
    kind_ = m->kind_;
  }

  ~MarshallDeputy() = default;
};

inline Marshal& operator>>(Marshal& m, MarshallDeputy& rhs) {
  struct timespec begin, end;
  clock_gettime(CLOCK_MONOTONIC, &begin);
  // Log_debug("==== inside Marshal& operator>>; checkpoint 0 @ %d", gettid());
  m >> rhs.kind_;
  // Log_debug("==== inside Marshal& operator>>; checkpoint 1 @ %d", gettid());
  rhs.CreateActualObjectFrom(m);
  // Log_debug("==== inside Marshal& operator>>; checkpoint 2 @ %d", gettid());
  clock_gettime(CLOCK_MONOTONIC, &end);
	// Log_debug("time of Marshal& operator>>: %d", end.tv_nsec-begin.tv_nsec);
  return m;
}

inline Marshal& operator<<(Marshal& m, const MarshallDeputy& rhs) {
  verify(rhs.kind_ != MarshallDeputy::UNKNOWN);
  // Log_debug("==== inside Marshal& operator<<; checkpoint 0 @ %d", gettid());
  m << rhs.kind_;
  verify(rhs.sp_data_); // must be non-empty
  // Log_debug("==== inside Marshal& operator<<; checkpoint 1 @ %d", gettid());
  rhs.sp_data_->ToMarshal(m); // #profile (crpc2) - 5.31%
  // Log_debug("==== inside Marshal& operator<<; checkpoint 2 @ %d", gettid());
  // clock_gettime(CLOCK_MONOTONIC, &end);
	// // Log_debug("time of Marshal& operator<<: %d", end.tv_nsec-begin.tv_nsec);
  return m;
}

} // namespace janus;
