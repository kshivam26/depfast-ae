#pragma once

#include "rrr.hpp"

#include <errno.h>


namespace janus {

struct ValueTimesPair {
    rrr::i64 value;
    rrr::i64 times;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ValueTimesPair& o) {
    m << o.value;
    m << o.times;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ValueTimesPair& o) {
    m >> o.value;
    m >> o.times;
    return m;
}

struct DepId {
    std::string str;
    rrr::i64 id;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const DepId& o) {
    m << o.str;
    m << o.id;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, DepId& o) {
    m >> o.str;
    m >> o.id;
    return m;
}

struct TxnInfoRes {
    rrr::i32 start_txn;
    rrr::i32 total_txn;
    rrr::i32 total_try;
    rrr::i32 commit_txn;
    rrr::i32 num_exhausted;
    std::vector<double> this_latency;
    std::vector<double> last_latency;
    std::vector<double> attempt_latency;
    std::vector<double> interval_latency;
    std::vector<double> all_interval_latency;
    std::vector<rrr::i32> num_try;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const TxnInfoRes& o) {
    m << o.start_txn;
    m << o.total_txn;
    m << o.total_try;
    m << o.commit_txn;
    m << o.num_exhausted;
    m << o.this_latency;
    m << o.last_latency;
    m << o.attempt_latency;
    m << o.interval_latency;
    m << o.all_interval_latency;
    m << o.num_try;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, TxnInfoRes& o) {
    m >> o.start_txn;
    m >> o.total_txn;
    m >> o.total_try;
    m >> o.commit_txn;
    m >> o.num_exhausted;
    m >> o.this_latency;
    m >> o.last_latency;
    m >> o.attempt_latency;
    m >> o.interval_latency;
    m >> o.all_interval_latency;
    m >> o.num_try;
    return m;
}

struct ServerResponse {
    std::map<std::string, ValueTimesPair> statistics;
    double cpu_util;
    rrr::i64 r_cnt_sum;
    rrr::i64 r_cnt_num;
    rrr::i64 r_sz_sum;
    rrr::i64 r_sz_num;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ServerResponse& o) {
    m << o.statistics;
    m << o.cpu_util;
    m << o.r_cnt_sum;
    m << o.r_cnt_num;
    m << o.r_sz_sum;
    m << o.r_sz_num;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ServerResponse& o) {
    m >> o.statistics;
    m >> o.cpu_util;
    m >> o.r_cnt_sum;
    m >> o.r_cnt_num;
    m >> o.r_sz_sum;
    m >> o.r_sz_num;
    return m;
}

struct ClientResponse {
    std::map<rrr::i32, TxnInfoRes> txn_info;
    rrr::i64 run_sec;
    rrr::i64 run_nsec;
    rrr::i64 period_sec;
    rrr::i64 period_nsec;
    rrr::i32 is_finish;
    rrr::i64 n_asking;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ClientResponse& o) {
    m << o.txn_info;
    m << o.run_sec;
    m << o.run_nsec;
    m << o.period_sec;
    m << o.period_nsec;
    m << o.is_finish;
    m << o.n_asking;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ClientResponse& o) {
    m >> o.txn_info;
    m >> o.run_sec;
    m >> o.run_nsec;
    m >> o.period_sec;
    m >> o.period_nsec;
    m >> o.is_finish;
    m >> o.n_asking;
    return m;
}

struct Profiling {
    double cpu_util;
    double tx_util;
    double rx_util;
    double mem_util;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const Profiling& o) {
    m << o.cpu_util;
    m << o.tx_util;
    m << o.rx_util;
    m << o.mem_util;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, Profiling& o) {
    m >> o.cpu_util;
    m >> o.tx_util;
    m >> o.rx_util;
    m >> o.mem_util;
    return m;
}

struct TxDispatchRequest {
    rrr::i32 id;
    rrr::i32 tx_type;
    std::vector<Value> input;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const TxDispatchRequest& o) {
    m << o.id;
    m << o.tx_type;
    m << o.input;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, TxDispatchRequest& o) {
    m >> o.id;
    m >> o.tx_type;
    m >> o.input;
    return m;
}

struct TxnDispatchResponse {
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const TxnDispatchResponse& o) {
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, TxnDispatchResponse& o) {
    return m;
}

struct AppendEntriesResult {
    uint64_t followerAppendOK;
    uint64_t followerCurrentTerm;
    uint64_t followerLastLogIndex;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const AppendEntriesResult& o) {
    m << o.followerAppendOK;
    m << o.followerCurrentTerm;
    m << o.followerLastLogIndex;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, AppendEntriesResult& o) {
    m >> o.followerAppendOK;
    m >> o.followerCurrentTerm;
    m >> o.followerLastLogIndex;
    return m;
}

struct ResultAdd {
    int64_t result;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ResultAdd& o) {
    m << o.result;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ResultAdd& o) {
    m >> o.result;
    return m;
}

class BenchmarkService: public rrr::Service {
public:
    enum {
        NOP = 0x415439f0,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(NOP, this, &BenchmarkService::__nop__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(NOP);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void nop(const std::string& st, rrr::DeferredReply* defer);
private:
    void __nop__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::string* in_0 = new std::string;
        req->m >> *in_0;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->nop(*in_0, __defer__);
    }
};

class BenchmarkProxy {
protected:
    rrr::Client* __cl__;
public:
    BenchmarkProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_nop(const std::string& st, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(BenchmarkService::NOP, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << st;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 nop(const std::string& st) {
        rrr::Future* __fu__ = this->async_nop(st);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

class CrpcBenchmarkService: public rrr::Service {
public:
    enum {
        CRPCADD = 0x198c1885,
        BROADCASTADD = 0x45b4757e,
        CRPCLARGEPAYLOAD = 0x6f439616,
        BROADCASTLARGEPAYLOAD = 0x57970110,
        NOP = 0x1d34de04,
        CRPC_NOP = 0x18632f05,
        NOP_LARGEPAYLOAD = 0x369d158b,
        CRPC_NOP_LARGEPAYLOAD = 0x12da1657,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(CRPCADD, this, &CrpcBenchmarkService::__CrpcAdd__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(BROADCASTADD, this, &CrpcBenchmarkService::__BroadcastAdd__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CRPCLARGEPAYLOAD, this, &CrpcBenchmarkService::__CrpcLargePayload__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(BROADCASTLARGEPAYLOAD, this, &CrpcBenchmarkService::__BroadcastLargePayload__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(NOP, this, &CrpcBenchmarkService::__nop__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CRPC_NOP, this, &CrpcBenchmarkService::__crpc_nop__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(NOP_LARGEPAYLOAD, this, &CrpcBenchmarkService::__nop_largePayload__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CRPC_NOP_LARGEPAYLOAD, this, &CrpcBenchmarkService::__crpc_nop_largePayload__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(CRPCADD);
        svr->unreg(BROADCASTADD);
        svr->unreg(CRPCLARGEPAYLOAD);
        svr->unreg(BROADCASTLARGEPAYLOAD);
        svr->unreg(NOP);
        svr->unreg(CRPC_NOP);
        svr->unreg(NOP_LARGEPAYLOAD);
        svr->unreg(CRPC_NOP_LARGEPAYLOAD);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void CrpcAdd(const uint64_t& id, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, rrr::DeferredReply* defer) = 0;
    virtual void BroadcastAdd(const int64_t& n, const int64_t& value1, const int64_t& value2, int64_t* result, rrr::DeferredReply* defer) = 0;
    virtual void CrpcLargePayload(const uint64_t& id, const std::string& str, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, rrr::DeferredReply* defer) = 0;
    virtual void BroadcastLargePayload(const int64_t& n, const std::string& str, int64_t* result, rrr::DeferredReply* defer) = 0;
    virtual void nop(const std::string&, rrr::DeferredReply* defer) = 0;
    virtual void crpc_nop(const std::string&, rrr::DeferredReply* defer) = 0;
    virtual void nop_largePayload(const std::string&, rrr::DeferredReply* defer) = 0;
    virtual void crpc_nop_largePayload(const std::string&, rrr::DeferredReply* defer) = 0;
private:
    void __CrpcAdd__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        int64_t* in_1 = new int64_t;
        req->m >> *in_1;
        int64_t* in_2 = new int64_t;
        req->m >> *in_2;
        std::vector<uint16_t>* in_3 = new std::vector<uint16_t>;
        req->m >> *in_3;
        std::vector<ResultAdd>* in_4 = new std::vector<ResultAdd>;
        req->m >> *in_4;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete in_4;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CrpcAdd(*in_0, *in_1, *in_2, *in_3, *in_4, __defer__);
    }
    void __BroadcastAdd__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        int64_t* in_0 = new int64_t;
        req->m >> *in_0;
        int64_t* in_1 = new int64_t;
        req->m >> *in_1;
        int64_t* in_2 = new int64_t;
        req->m >> *in_2;
        int64_t* out_0 = new int64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->BroadcastAdd(*in_0, *in_1, *in_2, out_0, __defer__);
    }
    void __CrpcLargePayload__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        std::string* in_1 = new std::string;
        req->m >> *in_1;
        std::vector<uint16_t>* in_2 = new std::vector<uint16_t>;
        req->m >> *in_2;
        std::vector<ResultAdd>* in_3 = new std::vector<ResultAdd>;
        req->m >> *in_3;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CrpcLargePayload(*in_0, *in_1, *in_2, *in_3, __defer__);
    }
    void __BroadcastLargePayload__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        int64_t* in_0 = new int64_t;
        req->m >> *in_0;
        std::string* in_1 = new std::string;
        req->m >> *in_1;
        int64_t* out_0 = new int64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->BroadcastLargePayload(*in_0, *in_1, out_0, __defer__);
    }
    void __nop__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::string* in_0 = new std::string;
        req->m >> *in_0;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->nop(*in_0, __defer__);
    }
    void __crpc_nop__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::string* in_0 = new std::string;
        req->m >> *in_0;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->crpc_nop(*in_0, __defer__);
    }
    void __nop_largePayload__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::string* in_0 = new std::string;
        req->m >> *in_0;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->nop_largePayload(*in_0, __defer__);
    }
    void __crpc_nop_largePayload__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::string* in_0 = new std::string;
        req->m >> *in_0;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->crpc_nop_largePayload(*in_0, __defer__);
    }
};

class CrpcBenchmarkProxy {
protected:
    rrr::Client* __cl__;
public:
    CrpcBenchmarkProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_CrpcAdd(const uint64_t& id, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CrpcBenchmarkService::CRPCADD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << value1;
            *__cl__ << value2;
            *__cl__ << addrChain;
            *__cl__ << state;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CrpcAdd(const uint64_t& id, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state) {
        rrr::Future* __fu__ = this->async_CrpcAdd(id, value1, value2, addrChain, state);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_BroadcastAdd(const int64_t& n, const int64_t& value1, const int64_t& value2, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CrpcBenchmarkService::BROADCASTADD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << n;
            *__cl__ << value1;
            *__cl__ << value2;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 BroadcastAdd(const int64_t& n, const int64_t& value1, const int64_t& value2, int64_t* result) {
        rrr::Future* __fu__ = this->async_BroadcastAdd(n, value1, value2);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *result;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_CrpcLargePayload(const uint64_t& id, const std::string& str, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CrpcBenchmarkService::CRPCLARGEPAYLOAD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << str;
            *__cl__ << addrChain;
            *__cl__ << state;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CrpcLargePayload(const uint64_t& id, const std::string& str, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state) {
        rrr::Future* __fu__ = this->async_CrpcLargePayload(id, str, addrChain, state);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_BroadcastLargePayload(const int64_t& n, const std::string& str, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CrpcBenchmarkService::BROADCASTLARGEPAYLOAD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << n;
            *__cl__ << str;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 BroadcastLargePayload(const int64_t& n, const std::string& str, int64_t* result) {
        rrr::Future* __fu__ = this->async_BroadcastLargePayload(n, str);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *result;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_nop(const std::string& in_0, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CrpcBenchmarkService::NOP, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << in_0;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 nop(const std::string& in_0) {
        rrr::Future* __fu__ = this->async_nop(in_0);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_crpc_nop(const std::string& in_0, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CrpcBenchmarkService::CRPC_NOP, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << in_0;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 crpc_nop(const std::string& in_0) {
        rrr::Future* __fu__ = this->async_crpc_nop(in_0);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_nop_largePayload(const std::string& in_0, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CrpcBenchmarkService::NOP_LARGEPAYLOAD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << in_0;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 nop_largePayload(const std::string& in_0) {
        rrr::Future* __fu__ = this->async_nop_largePayload(in_0);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_crpc_nop_largePayload(const std::string& in_0, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CrpcBenchmarkService::CRPC_NOP_LARGEPAYLOAD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << in_0;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 crpc_nop_largePayload(const std::string& in_0) {
        rrr::Future* __fu__ = this->async_crpc_nop_largePayload(in_0);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

class MultiPaxosService: public rrr::Service {
public:
    enum {
        FORWARD = 0x2cb7af38,
        PREPARE = 0x6ec732ab,
        ACCEPT = 0x102b5084,
        DECIDE = 0x5dcaa017,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(FORWARD, this, &MultiPaxosService::__Forward__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(PREPARE, this, &MultiPaxosService::__Prepare__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(ACCEPT, this, &MultiPaxosService::__Accept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(DECIDE, this, &MultiPaxosService::__Decide__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(FORWARD);
        svr->unreg(PREPARE);
        svr->unreg(ACCEPT);
        svr->unreg(DECIDE);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void Forward(const MarshallDeputy& cmd, const uint64_t& dep_id, uint64_t* coro_id, rrr::DeferredReply* defer) = 0;
    virtual void Prepare(const uint64_t& slot, const ballot_t& ballot, ballot_t* max_ballot, uint64_t* coro_id, rrr::DeferredReply* defer) = 0;
    virtual void Accept(const uint64_t& slot, const uint64_t& time, const ballot_t& ballot, const MarshallDeputy& cmd, ballot_t* max_ballot, uint64_t* coro_id, rrr::DeferredReply* defer) = 0;
    virtual void Decide(const uint64_t& slot, const ballot_t& ballot, const MarshallDeputy& cmd, rrr::DeferredReply* defer) = 0;
private:
    void __Forward__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        MarshallDeputy* in_0 = new MarshallDeputy;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        uint64_t* out_0 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Forward(*in_0, *in_1, out_0, __defer__);
    }
    void __Prepare__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        ballot_t* in_1 = new ballot_t;
        req->m >> *in_1;
        ballot_t* out_0 = new ballot_t;
        uint64_t* out_1 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Prepare(*in_0, *in_1, out_0, out_1, __defer__);
    }
    void __Accept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        ballot_t* in_2 = new ballot_t;
        req->m >> *in_2;
        MarshallDeputy* in_3 = new MarshallDeputy;
        req->m >> *in_3;
        ballot_t* out_0 = new ballot_t;
        uint64_t* out_1 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Accept(*in_0, *in_1, *in_2, *in_3, out_0, out_1, __defer__);
    }
    void __Decide__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        ballot_t* in_1 = new ballot_t;
        req->m >> *in_1;
        MarshallDeputy* in_2 = new MarshallDeputy;
        req->m >> *in_2;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Decide(*in_0, *in_1, *in_2, __defer__);
    }
};

class MultiPaxosProxy {
protected:
    rrr::Client* __cl__;
public:
    MultiPaxosProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_Forward(const MarshallDeputy& cmd, const uint64_t& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(MultiPaxosService::FORWARD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd;
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Forward(const MarshallDeputy& cmd, const uint64_t& dep_id, uint64_t* coro_id) {
        rrr::Future* __fu__ = this->async_Forward(cmd, dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *coro_id;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Prepare(const uint64_t& slot, const ballot_t& ballot, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(MultiPaxosService::PREPARE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << slot;
            *__cl__ << ballot;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Prepare(const uint64_t& slot, const ballot_t& ballot, ballot_t* max_ballot, uint64_t* coro_id) {
        rrr::Future* __fu__ = this->async_Prepare(slot, ballot);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *max_ballot;
            __fu__->get_reply() >> *coro_id;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Accept(const uint64_t& slot, const uint64_t& time, const ballot_t& ballot, const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(MultiPaxosService::ACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << slot;
            *__cl__ << time;
            *__cl__ << ballot;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Accept(const uint64_t& slot, const uint64_t& time, const ballot_t& ballot, const MarshallDeputy& cmd, ballot_t* max_ballot, uint64_t* coro_id) {
        rrr::Future* __fu__ = this->async_Accept(slot, time, ballot, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *max_ballot;
            __fu__->get_reply() >> *coro_id;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Decide(const uint64_t& slot, const ballot_t& ballot, const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(MultiPaxosService::DECIDE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Decide(const uint64_t& slot, const ballot_t& ballot, const MarshallDeputy& cmd) {
        rrr::Future* __fu__ = this->async_Decide(slot, ballot, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

class FpgaRaftService: public rrr::Service {
public:
    enum {
        HEARTBEAT = 0x6a377076,
        FORWARD = 0x4ca5001b,
        VOTE = 0x69a9f59b,
        VOTE2FPGA = 0x248604f2,
        APPENDENTRIES = 0x4f787572,
        CRPCAPPENDENTRIES = 0x21085748,
        CRPCAPPENDENTRIES3 = 0x3fa98e9b,
        APPENDENTRIES2 = 0x2b6237da,
        DECIDE = 0x524180e5,
        CRPC = 0x40db4c73,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(HEARTBEAT, this, &FpgaRaftService::__Heartbeat__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(FORWARD, this, &FpgaRaftService::__Forward__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(VOTE, this, &FpgaRaftService::__Vote__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(VOTE2FPGA, this, &FpgaRaftService::__Vote2FPGA__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(APPENDENTRIES, this, &FpgaRaftService::__AppendEntries__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CRPCAPPENDENTRIES, this, &FpgaRaftService::__CrpcAppendEntries__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CRPCAPPENDENTRIES3, this, &FpgaRaftService::__CrpcAppendEntries3__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(APPENDENTRIES2, this, &FpgaRaftService::__AppendEntries2__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(DECIDE, this, &FpgaRaftService::__Decide__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CRPC, this, &FpgaRaftService::__cRPC__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(HEARTBEAT);
        svr->unreg(FORWARD);
        svr->unreg(VOTE);
        svr->unreg(VOTE2FPGA);
        svr->unreg(APPENDENTRIES);
        svr->unreg(CRPCAPPENDENTRIES);
        svr->unreg(CRPCAPPENDENTRIES3);
        svr->unreg(APPENDENTRIES2);
        svr->unreg(DECIDE);
        svr->unreg(CRPC);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void Heartbeat(const uint64_t& leaderPrevLogIndex, const DepId& dep_id, uint64_t* followerPrevLogIndex, rrr::DeferredReply* defer) = 0;
    virtual void Forward(const MarshallDeputy& cmd, uint64_t* cmt_idx, rrr::DeferredReply* defer) = 0;
    virtual void Vote(const uint64_t& lst_log_idx, const ballot_t& lst_log_term, const parid_t& par_id, const ballot_t& cur_term, ballot_t* max_ballot, bool_t* vote_granted, rrr::DeferredReply* defer) = 0;
    virtual void Vote2FPGA(const uint64_t& lst_log_idx, const ballot_t& lst_log_term, const parid_t& par_id, const ballot_t& cur_term, ballot_t* max_ballot, bool_t* vote_granted, rrr::DeferredReply* defer) = 0;
    virtual void AppendEntries(const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, uint64_t* followerAppendOK, uint64_t* followerCurrentTerm, uint64_t* followerLastLogIndex, rrr::DeferredReply* defer) = 0;
    virtual void CrpcAppendEntries(const uint64_t& id, const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, const std::vector<AppendEntriesResult>& state, rrr::DeferredReply* defer) = 0;
    virtual void CrpcAppendEntries3(const uint64_t& id, const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, std::vector<AppendEntriesResult>* state, rrr::DeferredReply* defer) = 0;
    virtual void AppendEntries2(const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, uint64_t* followerAppendOK, uint64_t* followerCurrentTerm, uint64_t* followerLastLogIndex, rrr::DeferredReply* defer) = 0;
    virtual void Decide(const uint64_t& slot, const ballot_t& ballot, const DepId& dep_id, const MarshallDeputy& cmd, rrr::DeferredReply* defer) = 0;
    virtual void cRPC(const uint64_t& id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, const MarshallDeputy& state, rrr::DeferredReply* defer) = 0;
private:
    void __Heartbeat__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        DepId* in_1 = new DepId;
        req->m >> *in_1;
        uint64_t* out_0 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Heartbeat(*in_0, *in_1, out_0, __defer__);
    }
    void __Forward__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        MarshallDeputy* in_0 = new MarshallDeputy;
        req->m >> *in_0;
        uint64_t* out_0 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Forward(*in_0, out_0, __defer__);
    }
    void __Vote__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        ballot_t* in_1 = new ballot_t;
        req->m >> *in_1;
        parid_t* in_2 = new parid_t;
        req->m >> *in_2;
        ballot_t* in_3 = new ballot_t;
        req->m >> *in_3;
        ballot_t* out_0 = new ballot_t;
        bool_t* out_1 = new bool_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Vote(*in_0, *in_1, *in_2, *in_3, out_0, out_1, __defer__);
    }
    void __Vote2FPGA__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        ballot_t* in_1 = new ballot_t;
        req->m >> *in_1;
        parid_t* in_2 = new parid_t;
        req->m >> *in_2;
        ballot_t* in_3 = new ballot_t;
        req->m >> *in_3;
        ballot_t* out_0 = new ballot_t;
        bool_t* out_1 = new bool_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Vote2FPGA(*in_0, *in_1, *in_2, *in_3, out_0, out_1, __defer__);
    }
    void __AppendEntries__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        ballot_t* in_1 = new ballot_t;
        req->m >> *in_1;
        uint64_t* in_2 = new uint64_t;
        req->m >> *in_2;
        uint64_t* in_3 = new uint64_t;
        req->m >> *in_3;
        uint64_t* in_4 = new uint64_t;
        req->m >> *in_4;
        uint64_t* in_5 = new uint64_t;
        req->m >> *in_5;
        DepId* in_6 = new DepId;
        req->m >> *in_6;
        MarshallDeputy* in_7 = new MarshallDeputy;
        req->m >> *in_7;
        uint64_t* out_0 = new uint64_t;
        uint64_t* out_1 = new uint64_t;
        uint64_t* out_2 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete in_4;
            delete in_5;
            delete in_6;
            delete in_7;
            delete out_0;
            delete out_1;
            delete out_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->AppendEntries(*in_0, *in_1, *in_2, *in_3, *in_4, *in_5, *in_6, *in_7, out_0, out_1, out_2, __defer__);
    }
    void __CrpcAppendEntries__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        ballot_t* in_2 = new ballot_t;
        req->m >> *in_2;
        uint64_t* in_3 = new uint64_t;
        req->m >> *in_3;
        uint64_t* in_4 = new uint64_t;
        req->m >> *in_4;
        uint64_t* in_5 = new uint64_t;
        req->m >> *in_5;
        uint64_t* in_6 = new uint64_t;
        req->m >> *in_6;
        DepId* in_7 = new DepId;
        req->m >> *in_7;
        MarshallDeputy* in_8 = new MarshallDeputy;
        req->m >> *in_8;
        std::vector<uint16_t>* in_9 = new std::vector<uint16_t>;
        req->m >> *in_9;
        std::vector<AppendEntriesResult>* in_10 = new std::vector<AppendEntriesResult>;
        req->m >> *in_10;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete in_4;
            delete in_5;
            delete in_6;
            delete in_7;
            delete in_8;
            delete in_9;
            delete in_10;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CrpcAppendEntries(*in_0, *in_1, *in_2, *in_3, *in_4, *in_5, *in_6, *in_7, *in_8, *in_9, *in_10, __defer__);
    }
    void __CrpcAppendEntries3__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        ballot_t* in_2 = new ballot_t;
        req->m >> *in_2;
        uint64_t* in_3 = new uint64_t;
        req->m >> *in_3;
        uint64_t* in_4 = new uint64_t;
        req->m >> *in_4;
        uint64_t* in_5 = new uint64_t;
        req->m >> *in_5;
        uint64_t* in_6 = new uint64_t;
        req->m >> *in_6;
        DepId* in_7 = new DepId;
        req->m >> *in_7;
        MarshallDeputy* in_8 = new MarshallDeputy;
        req->m >> *in_8;
        std::vector<uint16_t>* in_9 = new std::vector<uint16_t>;
        req->m >> *in_9;
        std::vector<AppendEntriesResult>* out_0 = new std::vector<AppendEntriesResult>;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete in_4;
            delete in_5;
            delete in_6;
            delete in_7;
            delete in_8;
            delete in_9;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CrpcAppendEntries3(*in_0, *in_1, *in_2, *in_3, *in_4, *in_5, *in_6, *in_7, *in_8, *in_9, out_0, __defer__);
    }
    void __AppendEntries2__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        ballot_t* in_1 = new ballot_t;
        req->m >> *in_1;
        uint64_t* in_2 = new uint64_t;
        req->m >> *in_2;
        uint64_t* in_3 = new uint64_t;
        req->m >> *in_3;
        uint64_t* in_4 = new uint64_t;
        req->m >> *in_4;
        uint64_t* in_5 = new uint64_t;
        req->m >> *in_5;
        DepId* in_6 = new DepId;
        req->m >> *in_6;
        MarshallDeputy* in_7 = new MarshallDeputy;
        req->m >> *in_7;
        uint64_t* out_0 = new uint64_t;
        uint64_t* out_1 = new uint64_t;
        uint64_t* out_2 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete in_4;
            delete in_5;
            delete in_6;
            delete in_7;
            delete out_0;
            delete out_1;
            delete out_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->AppendEntries2(*in_0, *in_1, *in_2, *in_3, *in_4, *in_5, *in_6, *in_7, out_0, out_1, out_2, __defer__);
    }
    void __Decide__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        ballot_t* in_1 = new ballot_t;
        req->m >> *in_1;
        DepId* in_2 = new DepId;
        req->m >> *in_2;
        MarshallDeputy* in_3 = new MarshallDeputy;
        req->m >> *in_3;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Decide(*in_0, *in_1, *in_2, *in_3, __defer__);
    }
    void __cRPC__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        MarshallDeputy* in_1 = new MarshallDeputy;
        req->m >> *in_1;
        std::vector<uint16_t>* in_2 = new std::vector<uint16_t>;
        req->m >> *in_2;
        MarshallDeputy* in_3 = new MarshallDeputy;
        req->m >> *in_3;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->cRPC(*in_0, *in_1, *in_2, *in_3, __defer__);
    }
};

class FpgaRaftProxy {
protected:
    rrr::Client* __cl__;
public:
    FpgaRaftProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_Heartbeat(const uint64_t& leaderPrevLogIndex, const DepId& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::HEARTBEAT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << leaderPrevLogIndex;
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Heartbeat(const uint64_t& leaderPrevLogIndex, const DepId& dep_id, uint64_t* followerPrevLogIndex) {
        rrr::Future* __fu__ = this->async_Heartbeat(leaderPrevLogIndex, dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *followerPrevLogIndex;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Forward(const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::FORWARD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Forward(const MarshallDeputy& cmd, uint64_t* cmt_idx) {
        rrr::Future* __fu__ = this->async_Forward(cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *cmt_idx;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Vote(const uint64_t& lst_log_idx, const ballot_t& lst_log_term, const parid_t& par_id, const ballot_t& cur_term, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::VOTE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << lst_log_idx;
            *__cl__ << lst_log_term;
            *__cl__ << par_id;
            *__cl__ << cur_term;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Vote(const uint64_t& lst_log_idx, const ballot_t& lst_log_term, const parid_t& par_id, const ballot_t& cur_term, ballot_t* max_ballot, bool_t* vote_granted) {
        rrr::Future* __fu__ = this->async_Vote(lst_log_idx, lst_log_term, par_id, cur_term);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *max_ballot;
            __fu__->get_reply() >> *vote_granted;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Vote2FPGA(const uint64_t& lst_log_idx, const ballot_t& lst_log_term, const parid_t& par_id, const ballot_t& cur_term, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::VOTE2FPGA, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << lst_log_idx;
            *__cl__ << lst_log_term;
            *__cl__ << par_id;
            *__cl__ << cur_term;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Vote2FPGA(const uint64_t& lst_log_idx, const ballot_t& lst_log_term, const parid_t& par_id, const ballot_t& cur_term, ballot_t* max_ballot, bool_t* vote_granted) {
        rrr::Future* __fu__ = this->async_Vote2FPGA(lst_log_idx, lst_log_term, par_id, cur_term);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *max_ballot;
            __fu__->get_reply() >> *vote_granted;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_AppendEntries(const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::APPENDENTRIES, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << leaderCurrentTerm;
            *__cl__ << leaderPrevLogIndex;
            *__cl__ << leaderPrevLogTerm;
            *__cl__ << leaderCommitIndex;
            *__cl__ << dep_id;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 AppendEntries(const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, uint64_t* followerAppendOK, uint64_t* followerCurrentTerm, uint64_t* followerLastLogIndex) {
        rrr::Future* __fu__ = this->async_AppendEntries(slot, ballot, leaderCurrentTerm, leaderPrevLogIndex, leaderPrevLogTerm, leaderCommitIndex, dep_id, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *followerAppendOK;
            __fu__->get_reply() >> *followerCurrentTerm;
            __fu__->get_reply() >> *followerLastLogIndex;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_CrpcAppendEntries(const uint64_t& id, const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, const std::vector<AppendEntriesResult>& state, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::CRPCAPPENDENTRIES, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << leaderCurrentTerm;
            *__cl__ << leaderPrevLogIndex;
            *__cl__ << leaderPrevLogTerm;
            *__cl__ << leaderCommitIndex;
            *__cl__ << dep_id;
            *__cl__ << cmd;
            *__cl__ << addrChain;
            *__cl__ << state;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CrpcAppendEntries(const uint64_t& id, const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, const std::vector<AppendEntriesResult>& state) {
        rrr::Future* __fu__ = this->async_CrpcAppendEntries(id, slot, ballot, leaderCurrentTerm, leaderPrevLogIndex, leaderPrevLogTerm, leaderCommitIndex, dep_id, cmd, addrChain, state);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_CrpcAppendEntries3(const uint64_t& id, const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::CRPCAPPENDENTRIES3, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << leaderCurrentTerm;
            *__cl__ << leaderPrevLogIndex;
            *__cl__ << leaderPrevLogTerm;
            *__cl__ << leaderCommitIndex;
            *__cl__ << dep_id;
            *__cl__ << cmd;
            *__cl__ << addrChain;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CrpcAppendEntries3(const uint64_t& id, const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, std::vector<AppendEntriesResult>* state) {
        rrr::Future* __fu__ = this->async_CrpcAppendEntries3(id, slot, ballot, leaderCurrentTerm, leaderPrevLogIndex, leaderPrevLogTerm, leaderCommitIndex, dep_id, cmd, addrChain);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *state;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_AppendEntries2(const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::APPENDENTRIES2, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << leaderCurrentTerm;
            *__cl__ << leaderPrevLogIndex;
            *__cl__ << leaderPrevLogTerm;
            *__cl__ << leaderCommitIndex;
            *__cl__ << dep_id;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 AppendEntries2(const uint64_t& slot, const ballot_t& ballot, const uint64_t& leaderCurrentTerm, const uint64_t& leaderPrevLogIndex, const uint64_t& leaderPrevLogTerm, const uint64_t& leaderCommitIndex, const DepId& dep_id, const MarshallDeputy& cmd, uint64_t* followerAppendOK, uint64_t* followerCurrentTerm, uint64_t* followerLastLogIndex) {
        rrr::Future* __fu__ = this->async_AppendEntries2(slot, ballot, leaderCurrentTerm, leaderPrevLogIndex, leaderPrevLogTerm, leaderCommitIndex, dep_id, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *followerAppendOK;
            __fu__->get_reply() >> *followerCurrentTerm;
            __fu__->get_reply() >> *followerLastLogIndex;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Decide(const uint64_t& slot, const ballot_t& ballot, const DepId& dep_id, const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::DECIDE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << dep_id;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Decide(const uint64_t& slot, const ballot_t& ballot, const DepId& dep_id, const MarshallDeputy& cmd) {
        rrr::Future* __fu__ = this->async_Decide(slot, ballot, dep_id, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_cRPC(const uint64_t& id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, const MarshallDeputy& state, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(FpgaRaftService::CRPC, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << cmd;
            *__cl__ << addrChain;
            *__cl__ << state;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 cRPC(const uint64_t& id, const MarshallDeputy& cmd, const std::vector<uint16_t>& addrChain, const MarshallDeputy& state) {
        rrr::Future* __fu__ = this->async_cRPC(id, cmd, addrChain, state);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

class CopilotService: public rrr::Service {
public:
    enum {
        FORWARD = 0x6895a3ac,
        PREPARE = 0x3804948f,
        FASTACCEPT = 0x3f789584,
        ACCEPT = 0x238dd154,
        COMMIT = 0x36b180d0,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(FORWARD, this, &CopilotService::__Forward__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(PREPARE, this, &CopilotService::__Prepare__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(FASTACCEPT, this, &CopilotService::__FastAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(ACCEPT, this, &CopilotService::__Accept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(COMMIT, this, &CopilotService::__Commit__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(FORWARD);
        svr->unreg(PREPARE);
        svr->unreg(FASTACCEPT);
        svr->unreg(ACCEPT);
        svr->unreg(COMMIT);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void Forward(const MarshallDeputy& cmd, rrr::DeferredReply* defer) = 0;
    virtual void Prepare(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const DepId& dep_id, MarshallDeputy* ret_cmd, ballot_t* max_ballot, uint64_t* dep, status_t* status, rrr::DeferredReply* defer) = 0;
    virtual void FastAccept(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const uint64_t& dep, const MarshallDeputy& cmd, const DepId& dep_id, ballot_t* max_ballot, uint64_t* ret_dep, rrr::DeferredReply* defer) = 0;
    virtual void Accept(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const uint64_t& dep, const MarshallDeputy& cmd, const DepId& dep_id, ballot_t* max_ballot, rrr::DeferredReply* defer) = 0;
    virtual void Commit(const uint8_t& is_pilot, const uint64_t& slot, const uint64_t& dep, const MarshallDeputy& cmd, rrr::DeferredReply* defer) = 0;
private:
    void __Forward__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        MarshallDeputy* in_0 = new MarshallDeputy;
        req->m >> *in_0;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Forward(*in_0, __defer__);
    }
    void __Prepare__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint8_t* in_0 = new uint8_t;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        ballot_t* in_2 = new ballot_t;
        req->m >> *in_2;
        DepId* in_3 = new DepId;
        req->m >> *in_3;
        MarshallDeputy* out_0 = new MarshallDeputy;
        ballot_t* out_1 = new ballot_t;
        uint64_t* out_2 = new uint64_t;
        status_t* out_3 = new status_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
            *sconn << *out_3;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
            delete out_1;
            delete out_2;
            delete out_3;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Prepare(*in_0, *in_1, *in_2, *in_3, out_0, out_1, out_2, out_3, __defer__);
    }
    void __FastAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint8_t* in_0 = new uint8_t;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        ballot_t* in_2 = new ballot_t;
        req->m >> *in_2;
        uint64_t* in_3 = new uint64_t;
        req->m >> *in_3;
        MarshallDeputy* in_4 = new MarshallDeputy;
        req->m >> *in_4;
        DepId* in_5 = new DepId;
        req->m >> *in_5;
        ballot_t* out_0 = new ballot_t;
        uint64_t* out_1 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete in_4;
            delete in_5;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->FastAccept(*in_0, *in_1, *in_2, *in_3, *in_4, *in_5, out_0, out_1, __defer__);
    }
    void __Accept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint8_t* in_0 = new uint8_t;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        ballot_t* in_2 = new ballot_t;
        req->m >> *in_2;
        uint64_t* in_3 = new uint64_t;
        req->m >> *in_3;
        MarshallDeputy* in_4 = new MarshallDeputy;
        req->m >> *in_4;
        DepId* in_5 = new DepId;
        req->m >> *in_5;
        ballot_t* out_0 = new ballot_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete in_4;
            delete in_5;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Accept(*in_0, *in_1, *in_2, *in_3, *in_4, *in_5, out_0, __defer__);
    }
    void __Commit__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint8_t* in_0 = new uint8_t;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        uint64_t* in_2 = new uint64_t;
        req->m >> *in_2;
        MarshallDeputy* in_3 = new MarshallDeputy;
        req->m >> *in_3;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Commit(*in_0, *in_1, *in_2, *in_3, __defer__);
    }
};

class CopilotProxy {
protected:
    rrr::Client* __cl__;
public:
    CopilotProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_Forward(const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CopilotService::FORWARD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Forward(const MarshallDeputy& cmd) {
        rrr::Future* __fu__ = this->async_Forward(cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Prepare(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const DepId& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CopilotService::PREPARE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << is_pilot;
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Prepare(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const DepId& dep_id, MarshallDeputy* ret_cmd, ballot_t* max_ballot, uint64_t* dep, status_t* status) {
        rrr::Future* __fu__ = this->async_Prepare(is_pilot, slot, ballot, dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *ret_cmd;
            __fu__->get_reply() >> *max_ballot;
            __fu__->get_reply() >> *dep;
            __fu__->get_reply() >> *status;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_FastAccept(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const uint64_t& dep, const MarshallDeputy& cmd, const DepId& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CopilotService::FASTACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << is_pilot;
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << dep;
            *__cl__ << cmd;
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 FastAccept(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const uint64_t& dep, const MarshallDeputy& cmd, const DepId& dep_id, ballot_t* max_ballot, uint64_t* ret_dep) {
        rrr::Future* __fu__ = this->async_FastAccept(is_pilot, slot, ballot, dep, cmd, dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *max_ballot;
            __fu__->get_reply() >> *ret_dep;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Accept(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const uint64_t& dep, const MarshallDeputy& cmd, const DepId& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CopilotService::ACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << is_pilot;
            *__cl__ << slot;
            *__cl__ << ballot;
            *__cl__ << dep;
            *__cl__ << cmd;
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Accept(const uint8_t& is_pilot, const uint64_t& slot, const ballot_t& ballot, const uint64_t& dep, const MarshallDeputy& cmd, const DepId& dep_id, ballot_t* max_ballot) {
        rrr::Future* __fu__ = this->async_Accept(is_pilot, slot, ballot, dep, cmd, dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *max_ballot;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Commit(const uint8_t& is_pilot, const uint64_t& slot, const uint64_t& dep, const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(CopilotService::COMMIT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << is_pilot;
            *__cl__ << slot;
            *__cl__ << dep;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Commit(const uint8_t& is_pilot, const uint64_t& slot, const uint64_t& dep, const MarshallDeputy& cmd) {
        rrr::Future* __fu__ = this->async_Commit(is_pilot, slot, dep, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

class SampleCrpcService: public rrr::Service {
public:
    enum {
        CRPCADD = 0x17e839ec,
        BROADCASTADD = 0x54bdff18,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(CRPCADD, this, &SampleCrpcService::__CrpcAdd__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(BROADCASTADD, this, &SampleCrpcService::__BroadcastAdd__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(CRPCADD);
        svr->unreg(BROADCASTADD);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void CrpcAdd(const uint64_t& id, const int64_t& n, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, rrr::DeferredReply* defer) = 0;
    virtual void BroadcastAdd(const int64_t& n, const int64_t& value1, const int64_t& value2, int64_t* result, rrr::DeferredReply* defer) = 0;
private:
    void __CrpcAdd__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        int64_t* in_1 = new int64_t;
        req->m >> *in_1;
        int64_t* in_2 = new int64_t;
        req->m >> *in_2;
        int64_t* in_3 = new int64_t;
        req->m >> *in_3;
        std::vector<uint16_t>* in_4 = new std::vector<uint16_t>;
        req->m >> *in_4;
        std::vector<ResultAdd>* in_5 = new std::vector<ResultAdd>;
        req->m >> *in_5;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete in_4;
            delete in_5;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CrpcAdd(*in_0, *in_1, *in_2, *in_3, *in_4, *in_5, __defer__);
    }
    void __BroadcastAdd__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        int64_t* in_0 = new int64_t;
        req->m >> *in_0;
        int64_t* in_1 = new int64_t;
        req->m >> *in_1;
        int64_t* in_2 = new int64_t;
        req->m >> *in_2;
        int64_t* out_0 = new int64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->BroadcastAdd(*in_0, *in_1, *in_2, out_0, __defer__);
    }
};

class SampleCrpcProxy {
protected:
    rrr::Client* __cl__;
public:
    SampleCrpcProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_CrpcAdd(const uint64_t& id, const int64_t& n, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(SampleCrpcService::CRPCADD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << n;
            *__cl__ << value1;
            *__cl__ << value2;
            *__cl__ << addrChain;
            *__cl__ << state;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CrpcAdd(const uint64_t& id, const int64_t& n, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state) {
        rrr::Future* __fu__ = this->async_CrpcAdd(id, n, value1, value2, addrChain, state);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_BroadcastAdd(const int64_t& n, const int64_t& value1, const int64_t& value2, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(SampleCrpcService::BROADCASTADD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << n;
            *__cl__ << value1;
            *__cl__ << value2;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 BroadcastAdd(const int64_t& n, const int64_t& value1, const int64_t& value2, int64_t* result) {
        rrr::Future* __fu__ = this->async_BroadcastAdd(n, value1, value2);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *result;
        }
        __fu__->release();
        return __ret__;
    }
};

class ClassicService: public rrr::Service {
public:
    enum {
        MSGSTRING = 0x1391ab41,
        MSGMARSHALL = 0x4c2c10df,
        REELECT = 0x5dace8b4,
        DISPATCH = 0x19f55b56,
        PREPARE = 0x49d5e7cb,
        COMMIT = 0x6c07ffbc,
        ABORT = 0x24ec2db9,
        EARLYABORT = 0x57939370,
        UPGRADEEPOCH = 0x635cbdfe,
        TRUNCATEEPOCH = 0x10cb0133,
        ISLEADER = 0x4d089573,
        ISFPGALEADER = 0x1134a9f1,
        SIMPLECMD = 0x19cc3281,
        FAILOVERTRIG = 0x43dee2e3,
        RPC_NULL = 0x61577063,
        TAPIRACCEPT = 0x4aa2240b,
        TAPIRFASTACCEPT = 0x41153011,
        TAPIRDECIDE = 0x49355d37,
        CAROUSELREADANDPREPARE = 0x32e14801,
        CAROUSELACCEPT = 0x1de8c372,
        CAROUSELFASTACCEPT = 0x41362195,
        CAROUSELDECIDE = 0x5faa8218,
        RCCDISPATCH = 0x68f77c63,
        RCCFINISH = 0x57fd7747,
        RCCINQUIRE = 0x17047602,
        RCCDISPATCHRO = 0x3d7e3ea1,
        RCCINQUIREVALIDATION = 0x2bbdbd83,
        RCCNOTIFYGLOBALVALIDATION = 0x3343103e,
        JANUSDISPATCH = 0x46b5e32b,
        RCCCOMMIT = 0x47e94bdd,
        JANUSCOMMIT = 0x4697b48a,
        JANUSCOMMITWOGRAPH = 0x301d87d5,
        JANUSINQUIRE = 0x6e6da1ab,
        RCCPREACCEPT = 0x35d18bcf,
        JANUSPREACCEPT = 0x419bb0ce,
        JANUSPREACCEPTWOGRAPH = 0x54245345,
        RCCACCEPT = 0x29ba23bc,
        JANUSACCEPT = 0x18beb31f,
        PREACCEPTFEBRUUS = 0x695e48a8,
        ACCEPTFEBRUUS = 0x465e2dd6,
        COMMITFEBRUUS = 0x299d4aa5,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(MSGSTRING, this, &ClassicService::__MsgString__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(MSGMARSHALL, this, &ClassicService::__MsgMarshall__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(REELECT, this, &ClassicService::__ReElect__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(DISPATCH, this, &ClassicService::__Dispatch__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(PREPARE, this, &ClassicService::__Prepare__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(COMMIT, this, &ClassicService::__Commit__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(ABORT, this, &ClassicService::__Abort__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(EARLYABORT, this, &ClassicService::__EarlyAbort__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(UPGRADEEPOCH, this, &ClassicService::__UpgradeEpoch__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(TRUNCATEEPOCH, this, &ClassicService::__TruncateEpoch__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(ISLEADER, this, &ClassicService::__IsLeader__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(ISFPGALEADER, this, &ClassicService::__IsFPGALeader__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(SIMPLECMD, this, &ClassicService::__SimpleCmd__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(FAILOVERTRIG, this, &ClassicService::__FailOverTrig__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RPC_NULL, this, &ClassicService::__rpc_null__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(TAPIRACCEPT, this, &ClassicService::__TapirAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(TAPIRFASTACCEPT, this, &ClassicService::__TapirFastAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(TAPIRDECIDE, this, &ClassicService::__TapirDecide__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CAROUSELREADANDPREPARE, this, &ClassicService::__CarouselReadAndPrepare__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CAROUSELACCEPT, this, &ClassicService::__CarouselAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CAROUSELFASTACCEPT, this, &ClassicService::__CarouselFastAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CAROUSELDECIDE, this, &ClassicService::__CarouselDecide__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCDISPATCH, this, &ClassicService::__RccDispatch__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCFINISH, this, &ClassicService::__RccFinish__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCINQUIRE, this, &ClassicService::__RccInquire__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCDISPATCHRO, this, &ClassicService::__RccDispatchRo__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCINQUIREVALIDATION, this, &ClassicService::__RccInquireValidation__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCNOTIFYGLOBALVALIDATION, this, &ClassicService::__RccNotifyGlobalValidation__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(JANUSDISPATCH, this, &ClassicService::__JanusDispatch__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCCOMMIT, this, &ClassicService::__RccCommit__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(JANUSCOMMIT, this, &ClassicService::__JanusCommit__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(JANUSCOMMITWOGRAPH, this, &ClassicService::__JanusCommitWoGraph__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(JANUSINQUIRE, this, &ClassicService::__JanusInquire__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCPREACCEPT, this, &ClassicService::__RccPreAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(JANUSPREACCEPT, this, &ClassicService::__JanusPreAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(JANUSPREACCEPTWOGRAPH, this, &ClassicService::__JanusPreAcceptWoGraph__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCCACCEPT, this, &ClassicService::__RccAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(JANUSACCEPT, this, &ClassicService::__JanusAccept__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(PREACCEPTFEBRUUS, this, &ClassicService::__PreAcceptFebruus__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(ACCEPTFEBRUUS, this, &ClassicService::__AcceptFebruus__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(COMMITFEBRUUS, this, &ClassicService::__CommitFebruus__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(MSGSTRING);
        svr->unreg(MSGMARSHALL);
        svr->unreg(REELECT);
        svr->unreg(DISPATCH);
        svr->unreg(PREPARE);
        svr->unreg(COMMIT);
        svr->unreg(ABORT);
        svr->unreg(EARLYABORT);
        svr->unreg(UPGRADEEPOCH);
        svr->unreg(TRUNCATEEPOCH);
        svr->unreg(ISLEADER);
        svr->unreg(ISFPGALEADER);
        svr->unreg(SIMPLECMD);
        svr->unreg(FAILOVERTRIG);
        svr->unreg(RPC_NULL);
        svr->unreg(TAPIRACCEPT);
        svr->unreg(TAPIRFASTACCEPT);
        svr->unreg(TAPIRDECIDE);
        svr->unreg(CAROUSELREADANDPREPARE);
        svr->unreg(CAROUSELACCEPT);
        svr->unreg(CAROUSELFASTACCEPT);
        svr->unreg(CAROUSELDECIDE);
        svr->unreg(RCCDISPATCH);
        svr->unreg(RCCFINISH);
        svr->unreg(RCCINQUIRE);
        svr->unreg(RCCDISPATCHRO);
        svr->unreg(RCCINQUIREVALIDATION);
        svr->unreg(RCCNOTIFYGLOBALVALIDATION);
        svr->unreg(JANUSDISPATCH);
        svr->unreg(RCCCOMMIT);
        svr->unreg(JANUSCOMMIT);
        svr->unreg(JANUSCOMMITWOGRAPH);
        svr->unreg(JANUSINQUIRE);
        svr->unreg(RCCPREACCEPT);
        svr->unreg(JANUSPREACCEPT);
        svr->unreg(JANUSPREACCEPTWOGRAPH);
        svr->unreg(RCCACCEPT);
        svr->unreg(JANUSACCEPT);
        svr->unreg(PREACCEPTFEBRUUS);
        svr->unreg(ACCEPTFEBRUUS);
        svr->unreg(COMMITFEBRUUS);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void MsgString(const std::string& arg, std::string* ret, rrr::DeferredReply* defer) = 0;
    virtual void MsgMarshall(const MarshallDeputy& arg, MarshallDeputy* ret, rrr::DeferredReply* defer) = 0;
    virtual void ReElect(bool_t* success, rrr::DeferredReply* defer) = 0;
    virtual void Dispatch(const rrr::i64& tid, const DepId& dep_id, const MarshallDeputy& cmd, rrr::i32* res, TxnOutput* output, uint64_t* coro_id, rrr::DeferredReply* defer) = 0;
    virtual void Prepare(const rrr::i64& tid, const std::vector<rrr::i32>& sids, const DepId& dep_id, rrr::i32* res, bool_t* slow, uint64_t* coro_id, rrr::DeferredReply* defer) = 0;
    virtual void Commit(const rrr::i64& tid, const DepId& dep_id, rrr::i32* res, bool_t* slow, uint64_t* coro_id, Profiling* profile, rrr::DeferredReply* defer) = 0;
    virtual void Abort(const rrr::i64& tid, const DepId& dep_id, rrr::i32* res, bool_t* slow, uint64_t* coro_id, Profiling* profile, rrr::DeferredReply* defer) = 0;
    virtual void EarlyAbort(const rrr::i64& tid, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void UpgradeEpoch(const uint32_t& curr_epoch, int32_t* res, rrr::DeferredReply* defer) = 0;
    virtual void TruncateEpoch(const uint32_t& old_epoch, rrr::DeferredReply* defer) = 0;
    virtual void IsLeader(const locid_t& cur_pause, bool_t* is_leader, rrr::DeferredReply* defer) = 0;
    virtual void IsFPGALeader(const locid_t& cur_pause, bool_t* is_leader, rrr::DeferredReply* defer) = 0;
    virtual void SimpleCmd(const SimpleCommand& cmd, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void FailOverTrig(const bool_t& pause, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void rpc_null(rrr::DeferredReply* defer) = 0;
    virtual void TapirAccept(const uint64_t& cmd_id, const int64_t& ballot, const int32_t& decision, rrr::DeferredReply* defer) = 0;
    virtual void TapirFastAccept(const uint64_t& cmd_id, const std::vector<SimpleCommand>& txn_cmds, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void TapirDecide(const uint64_t& cmd_id, const rrr::i32& commit, rrr::DeferredReply* defer) = 0;
    virtual void CarouselReadAndPrepare(const rrr::i64& tid, const MarshallDeputy& cmd, const bool_t& leader, rrr::i32* res, TxnOutput* output, rrr::DeferredReply* defer) = 0;
    virtual void CarouselAccept(const uint64_t& cmd_id, const int64_t& ballot, const int32_t& decision, rrr::DeferredReply* defer) = 0;
    virtual void CarouselFastAccept(const uint64_t& cmd_id, const std::vector<SimpleCommand>& txn_cmds, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void CarouselDecide(const uint64_t& cmd_id, const rrr::i32& commit, rrr::DeferredReply* defer) = 0;
    virtual void RccDispatch(const std::vector<SimpleCommand>& cmd, rrr::i32* res, TxnOutput* output, MarshallDeputy* md_graph, rrr::DeferredReply* defer) = 0;
    virtual void RccFinish(const cmdid_t& id, const MarshallDeputy& md_graph, std::map<uint32_t, std::map<int32_t, Value>>* outputs, rrr::DeferredReply* defer) = 0;
    virtual void RccInquire(const txnid_t& txn_id, const int32_t& rank, std::map<uint64_t, parent_set_t>*, rrr::DeferredReply* defer) = 0;
    virtual void RccDispatchRo(const SimpleCommand& cmd, std::map<rrr::i32, Value>* output, rrr::DeferredReply* defer) = 0;
    virtual void RccInquireValidation(const txid_t& tx_id, const int32_t& rank, int32_t* res, rrr::DeferredReply* defer) = 0;
    virtual void RccNotifyGlobalValidation(const txid_t& tx_id, const int32_t& rank, const int32_t& res, rrr::DeferredReply* defer) = 0;
    virtual void JanusDispatch(const std::vector<SimpleCommand>& cmd, rrr::i32* res, TxnOutput* output, MarshallDeputy* ret_graph, rrr::DeferredReply* defer) = 0;
    virtual void RccCommit(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, const parent_set_t& parents, int32_t* res, TxnOutput* output, rrr::DeferredReply* defer) = 0;
    virtual void JanusCommit(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, const MarshallDeputy& graph, int32_t* res, TxnOutput* output, rrr::DeferredReply* defer) = 0;
    virtual void JanusCommitWoGraph(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, int32_t* res, TxnOutput* output, rrr::DeferredReply* defer) = 0;
    virtual void JanusInquire(const epoch_t& epoch, const txnid_t& txn_id, MarshallDeputy* ret_graph, rrr::DeferredReply* defer) = 0;
    virtual void RccPreAccept(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, rrr::i32* res, parent_set_t* x, rrr::DeferredReply* defer) = 0;
    virtual void JanusPreAccept(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, const MarshallDeputy& graph, rrr::i32* res, MarshallDeputy* ret_graph, rrr::DeferredReply* defer) = 0;
    virtual void JanusPreAcceptWoGraph(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, rrr::i32* res, MarshallDeputy* ret_graph, rrr::DeferredReply* defer) = 0;
    virtual void RccAccept(const cmdid_t& txn_id, const rrr::i32& rank, const ballot_t& ballot, const parent_set_t& p, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void JanusAccept(const cmdid_t& txn_id, const rrr::i32& rank, const ballot_t& ballot, const MarshallDeputy& graph, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void PreAcceptFebruus(const txid_t& tx_id, rrr::i32* ret, uint64_t* timestamp, rrr::DeferredReply* defer) = 0;
    virtual void AcceptFebruus(const txid_t& tx_id, const ballot_t& ballot, const uint64_t& timestamp, rrr::i32* ret, rrr::DeferredReply* defer) = 0;
    virtual void CommitFebruus(const txid_t& tx_id, const uint64_t& timestamp, rrr::i32* ret, rrr::DeferredReply* defer) = 0;
private:
    void __MsgString__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::string* in_0 = new std::string;
        req->m >> *in_0;
        std::string* out_0 = new std::string;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->MsgString(*in_0, out_0, __defer__);
    }
    void __MsgMarshall__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        MarshallDeputy* in_0 = new MarshallDeputy;
        req->m >> *in_0;
        MarshallDeputy* out_0 = new MarshallDeputy;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->MsgMarshall(*in_0, out_0, __defer__);
    }
    void __ReElect__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        bool_t* out_0 = new bool_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->ReElect(out_0, __defer__);
    }
    void __Dispatch__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        DepId* in_1 = new DepId;
        req->m >> *in_1;
        MarshallDeputy* in_2 = new MarshallDeputy;
        req->m >> *in_2;
        rrr::i32* out_0 = new rrr::i32;
        TxnOutput* out_1 = new TxnOutput;
        uint64_t* out_2 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
            delete out_1;
            delete out_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Dispatch(*in_0, *in_1, *in_2, out_0, out_1, out_2, __defer__);
    }
    void __Prepare__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        std::vector<rrr::i32>* in_1 = new std::vector<rrr::i32>;
        req->m >> *in_1;
        DepId* in_2 = new DepId;
        req->m >> *in_2;
        rrr::i32* out_0 = new rrr::i32;
        bool_t* out_1 = new bool_t;
        uint64_t* out_2 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
            delete out_1;
            delete out_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Prepare(*in_0, *in_1, *in_2, out_0, out_1, out_2, __defer__);
    }
    void __Commit__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        DepId* in_1 = new DepId;
        req->m >> *in_1;
        rrr::i32* out_0 = new rrr::i32;
        bool_t* out_1 = new bool_t;
        uint64_t* out_2 = new uint64_t;
        Profiling* out_3 = new Profiling;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
            *sconn << *out_3;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
            delete out_1;
            delete out_2;
            delete out_3;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Commit(*in_0, *in_1, out_0, out_1, out_2, out_3, __defer__);
    }
    void __Abort__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        DepId* in_1 = new DepId;
        req->m >> *in_1;
        rrr::i32* out_0 = new rrr::i32;
        bool_t* out_1 = new bool_t;
        uint64_t* out_2 = new uint64_t;
        Profiling* out_3 = new Profiling;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
            *sconn << *out_3;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
            delete out_1;
            delete out_2;
            delete out_3;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->Abort(*in_0, *in_1, out_0, out_1, out_2, out_3, __defer__);
    }
    void __EarlyAbort__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->EarlyAbort(*in_0, out_0, __defer__);
    }
    void __UpgradeEpoch__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint32_t* in_0 = new uint32_t;
        req->m >> *in_0;
        int32_t* out_0 = new int32_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->UpgradeEpoch(*in_0, out_0, __defer__);
    }
    void __TruncateEpoch__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint32_t* in_0 = new uint32_t;
        req->m >> *in_0;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->TruncateEpoch(*in_0, __defer__);
    }
    void __IsLeader__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        locid_t* in_0 = new locid_t;
        req->m >> *in_0;
        bool_t* out_0 = new bool_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->IsLeader(*in_0, out_0, __defer__);
    }
    void __IsFPGALeader__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        locid_t* in_0 = new locid_t;
        req->m >> *in_0;
        bool_t* out_0 = new bool_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->IsFPGALeader(*in_0, out_0, __defer__);
    }
    void __SimpleCmd__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        SimpleCommand* in_0 = new SimpleCommand;
        req->m >> *in_0;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->SimpleCmd(*in_0, out_0, __defer__);
    }
    void __FailOverTrig__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        bool_t* in_0 = new bool_t;
        req->m >> *in_0;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->FailOverTrig(*in_0, out_0, __defer__);
    }
    void __rpc_null__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->rpc_null(__defer__);
    }
    void __TapirAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        int64_t* in_1 = new int64_t;
        req->m >> *in_1;
        int32_t* in_2 = new int32_t;
        req->m >> *in_2;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->TapirAccept(*in_0, *in_1, *in_2, __defer__);
    }
    void __TapirFastAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        std::vector<SimpleCommand>* in_1 = new std::vector<SimpleCommand>;
        req->m >> *in_1;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->TapirFastAccept(*in_0, *in_1, out_0, __defer__);
    }
    void __TapirDecide__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        rrr::i32* in_1 = new rrr::i32;
        req->m >> *in_1;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->TapirDecide(*in_0, *in_1, __defer__);
    }
    void __CarouselReadAndPrepare__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        MarshallDeputy* in_1 = new MarshallDeputy;
        req->m >> *in_1;
        bool_t* in_2 = new bool_t;
        req->m >> *in_2;
        rrr::i32* out_0 = new rrr::i32;
        TxnOutput* out_1 = new TxnOutput;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CarouselReadAndPrepare(*in_0, *in_1, *in_2, out_0, out_1, __defer__);
    }
    void __CarouselAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        int64_t* in_1 = new int64_t;
        req->m >> *in_1;
        int32_t* in_2 = new int32_t;
        req->m >> *in_2;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CarouselAccept(*in_0, *in_1, *in_2, __defer__);
    }
    void __CarouselFastAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        std::vector<SimpleCommand>* in_1 = new std::vector<SimpleCommand>;
        req->m >> *in_1;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CarouselFastAccept(*in_0, *in_1, out_0, __defer__);
    }
    void __CarouselDecide__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        uint64_t* in_0 = new uint64_t;
        req->m >> *in_0;
        rrr::i32* in_1 = new rrr::i32;
        req->m >> *in_1;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CarouselDecide(*in_0, *in_1, __defer__);
    }
    void __RccDispatch__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::vector<SimpleCommand>* in_0 = new std::vector<SimpleCommand>;
        req->m >> *in_0;
        rrr::i32* out_0 = new rrr::i32;
        TxnOutput* out_1 = new TxnOutput;
        MarshallDeputy* out_2 = new MarshallDeputy;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
            delete out_1;
            delete out_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccDispatch(*in_0, out_0, out_1, out_2, __defer__);
    }
    void __RccFinish__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        MarshallDeputy* in_1 = new MarshallDeputy;
        req->m >> *in_1;
        std::map<uint32_t, std::map<int32_t, Value>>* out_0 = new std::map<uint32_t, std::map<int32_t, Value>>;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccFinish(*in_0, *in_1, out_0, __defer__);
    }
    void __RccInquire__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        txnid_t* in_0 = new txnid_t;
        req->m >> *in_0;
        int32_t* in_1 = new int32_t;
        req->m >> *in_1;
        std::map<uint64_t, parent_set_t>* out_0 = new std::map<uint64_t, parent_set_t>;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccInquire(*in_0, *in_1, out_0, __defer__);
    }
    void __RccDispatchRo__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        SimpleCommand* in_0 = new SimpleCommand;
        req->m >> *in_0;
        std::map<rrr::i32, Value>* out_0 = new std::map<rrr::i32, Value>;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccDispatchRo(*in_0, out_0, __defer__);
    }
    void __RccInquireValidation__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        txid_t* in_0 = new txid_t;
        req->m >> *in_0;
        int32_t* in_1 = new int32_t;
        req->m >> *in_1;
        int32_t* out_0 = new int32_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccInquireValidation(*in_0, *in_1, out_0, __defer__);
    }
    void __RccNotifyGlobalValidation__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        txid_t* in_0 = new txid_t;
        req->m >> *in_0;
        int32_t* in_1 = new int32_t;
        req->m >> *in_1;
        int32_t* in_2 = new int32_t;
        req->m >> *in_2;
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccNotifyGlobalValidation(*in_0, *in_1, *in_2, __defer__);
    }
    void __JanusDispatch__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::vector<SimpleCommand>* in_0 = new std::vector<SimpleCommand>;
        req->m >> *in_0;
        rrr::i32* out_0 = new rrr::i32;
        TxnOutput* out_1 = new TxnOutput;
        MarshallDeputy* out_2 = new MarshallDeputy;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
            *sconn << *out_2;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
            delete out_1;
            delete out_2;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->JanusDispatch(*in_0, out_0, out_1, out_2, __defer__);
    }
    void __RccCommit__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        rank_t* in_1 = new rank_t;
        req->m >> *in_1;
        int32_t* in_2 = new int32_t;
        req->m >> *in_2;
        parent_set_t* in_3 = new parent_set_t;
        req->m >> *in_3;
        int32_t* out_0 = new int32_t;
        TxnOutput* out_1 = new TxnOutput;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccCommit(*in_0, *in_1, *in_2, *in_3, out_0, out_1, __defer__);
    }
    void __JanusCommit__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        rank_t* in_1 = new rank_t;
        req->m >> *in_1;
        int32_t* in_2 = new int32_t;
        req->m >> *in_2;
        MarshallDeputy* in_3 = new MarshallDeputy;
        req->m >> *in_3;
        int32_t* out_0 = new int32_t;
        TxnOutput* out_1 = new TxnOutput;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->JanusCommit(*in_0, *in_1, *in_2, *in_3, out_0, out_1, __defer__);
    }
    void __JanusCommitWoGraph__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        rank_t* in_1 = new rank_t;
        req->m >> *in_1;
        int32_t* in_2 = new int32_t;
        req->m >> *in_2;
        int32_t* out_0 = new int32_t;
        TxnOutput* out_1 = new TxnOutput;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->JanusCommitWoGraph(*in_0, *in_1, *in_2, out_0, out_1, __defer__);
    }
    void __JanusInquire__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        epoch_t* in_0 = new epoch_t;
        req->m >> *in_0;
        txnid_t* in_1 = new txnid_t;
        req->m >> *in_1;
        MarshallDeputy* out_0 = new MarshallDeputy;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->JanusInquire(*in_0, *in_1, out_0, __defer__);
    }
    void __RccPreAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        rank_t* in_1 = new rank_t;
        req->m >> *in_1;
        std::vector<SimpleCommand>* in_2 = new std::vector<SimpleCommand>;
        req->m >> *in_2;
        rrr::i32* out_0 = new rrr::i32;
        parent_set_t* out_1 = new parent_set_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccPreAccept(*in_0, *in_1, *in_2, out_0, out_1, __defer__);
    }
    void __JanusPreAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        rank_t* in_1 = new rank_t;
        req->m >> *in_1;
        std::vector<SimpleCommand>* in_2 = new std::vector<SimpleCommand>;
        req->m >> *in_2;
        MarshallDeputy* in_3 = new MarshallDeputy;
        req->m >> *in_3;
        rrr::i32* out_0 = new rrr::i32;
        MarshallDeputy* out_1 = new MarshallDeputy;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->JanusPreAccept(*in_0, *in_1, *in_2, *in_3, out_0, out_1, __defer__);
    }
    void __JanusPreAcceptWoGraph__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        rank_t* in_1 = new rank_t;
        req->m >> *in_1;
        std::vector<SimpleCommand>* in_2 = new std::vector<SimpleCommand>;
        req->m >> *in_2;
        rrr::i32* out_0 = new rrr::i32;
        MarshallDeputy* out_1 = new MarshallDeputy;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->JanusPreAcceptWoGraph(*in_0, *in_1, *in_2, out_0, out_1, __defer__);
    }
    void __RccAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        rrr::i32* in_1 = new rrr::i32;
        req->m >> *in_1;
        ballot_t* in_2 = new ballot_t;
        req->m >> *in_2;
        parent_set_t* in_3 = new parent_set_t;
        req->m >> *in_3;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->RccAccept(*in_0, *in_1, *in_2, *in_3, out_0, __defer__);
    }
    void __JanusAccept__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        cmdid_t* in_0 = new cmdid_t;
        req->m >> *in_0;
        rrr::i32* in_1 = new rrr::i32;
        req->m >> *in_1;
        ballot_t* in_2 = new ballot_t;
        req->m >> *in_2;
        MarshallDeputy* in_3 = new MarshallDeputy;
        req->m >> *in_3;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete in_3;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->JanusAccept(*in_0, *in_1, *in_2, *in_3, out_0, __defer__);
    }
    void __PreAcceptFebruus__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        txid_t* in_0 = new txid_t;
        req->m >> *in_0;
        rrr::i32* out_0 = new rrr::i32;
        uint64_t* out_1 = new uint64_t;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->PreAcceptFebruus(*in_0, out_0, out_1, __defer__);
    }
    void __AcceptFebruus__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        txid_t* in_0 = new txid_t;
        req->m >> *in_0;
        ballot_t* in_1 = new ballot_t;
        req->m >> *in_1;
        uint64_t* in_2 = new uint64_t;
        req->m >> *in_2;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->AcceptFebruus(*in_0, *in_1, *in_2, out_0, __defer__);
    }
    void __CommitFebruus__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        txid_t* in_0 = new txid_t;
        req->m >> *in_0;
        uint64_t* in_1 = new uint64_t;
        req->m >> *in_1;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->CommitFebruus(*in_0, *in_1, out_0, __defer__);
    }
};

class ClassicProxy {
protected:
    rrr::Client* __cl__;
public:
    ClassicProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_MsgString(const std::string& arg, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::MSGSTRING, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << arg;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 MsgString(const std::string& arg, std::string* ret) {
        rrr::Future* __fu__ = this->async_MsgString(arg);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *ret;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_MsgMarshall(const MarshallDeputy& arg, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::MSGMARSHALL, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << arg;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 MsgMarshall(const MarshallDeputy& arg, MarshallDeputy* ret) {
        rrr::Future* __fu__ = this->async_MsgMarshall(arg);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *ret;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_ReElect(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::REELECT, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 ReElect(bool_t* success) {
        rrr::Future* __fu__ = this->async_ReElect();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *success;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Dispatch(const rrr::i64& tid, const DepId& dep_id, const MarshallDeputy& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::DISPATCH, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
            *__cl__ << dep_id;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Dispatch(const rrr::i64& tid, const DepId& dep_id, const MarshallDeputy& cmd, rrr::i32* res, TxnOutput* output, uint64_t* coro_id) {
        rrr::Future* __fu__ = this->async_Dispatch(tid, dep_id, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
            __fu__->get_reply() >> *coro_id;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Prepare(const rrr::i64& tid, const std::vector<rrr::i32>& sids, const DepId& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::PREPARE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
            *__cl__ << sids;
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Prepare(const rrr::i64& tid, const std::vector<rrr::i32>& sids, const DepId& dep_id, rrr::i32* res, bool_t* slow, uint64_t* coro_id) {
        rrr::Future* __fu__ = this->async_Prepare(tid, sids, dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *slow;
            __fu__->get_reply() >> *coro_id;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Commit(const rrr::i64& tid, const DepId& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::COMMIT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Commit(const rrr::i64& tid, const DepId& dep_id, rrr::i32* res, bool_t* slow, uint64_t* coro_id, Profiling* profile) {
        rrr::Future* __fu__ = this->async_Commit(tid, dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *slow;
            __fu__->get_reply() >> *coro_id;
            __fu__->get_reply() >> *profile;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_Abort(const rrr::i64& tid, const DepId& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::ABORT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 Abort(const rrr::i64& tid, const DepId& dep_id, rrr::i32* res, bool_t* slow, uint64_t* coro_id, Profiling* profile) {
        rrr::Future* __fu__ = this->async_Abort(tid, dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *slow;
            __fu__->get_reply() >> *coro_id;
            __fu__->get_reply() >> *profile;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_EarlyAbort(const rrr::i64& tid, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::EARLYABORT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 EarlyAbort(const rrr::i64& tid, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_EarlyAbort(tid);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_UpgradeEpoch(const uint32_t& curr_epoch, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::UPGRADEEPOCH, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << curr_epoch;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 UpgradeEpoch(const uint32_t& curr_epoch, int32_t* res) {
        rrr::Future* __fu__ = this->async_UpgradeEpoch(curr_epoch);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_TruncateEpoch(const uint32_t& old_epoch, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::TRUNCATEEPOCH, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << old_epoch;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 TruncateEpoch(const uint32_t& old_epoch) {
        rrr::Future* __fu__ = this->async_TruncateEpoch(old_epoch);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_IsLeader(const locid_t& cur_pause, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::ISLEADER, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cur_pause;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 IsLeader(const locid_t& cur_pause, bool_t* is_leader) {
        rrr::Future* __fu__ = this->async_IsLeader(cur_pause);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *is_leader;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_IsFPGALeader(const locid_t& cur_pause, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::ISFPGALEADER, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cur_pause;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 IsFPGALeader(const locid_t& cur_pause, bool_t* is_leader) {
        rrr::Future* __fu__ = this->async_IsFPGALeader(cur_pause);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *is_leader;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_SimpleCmd(const SimpleCommand& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::SIMPLECMD, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 SimpleCmd(const SimpleCommand& cmd, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_SimpleCmd(cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_FailOverTrig(const bool_t& pause, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::FAILOVERTRIG, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << pause;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 FailOverTrig(const bool_t& pause, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_FailOverTrig(pause);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_rpc_null(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RPC_NULL, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 rpc_null() {
        rrr::Future* __fu__ = this->async_rpc_null();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_TapirAccept(const uint64_t& cmd_id, const int64_t& ballot, const int32_t& decision, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::TAPIRACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd_id;
            *__cl__ << ballot;
            *__cl__ << decision;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 TapirAccept(const uint64_t& cmd_id, const int64_t& ballot, const int32_t& decision) {
        rrr::Future* __fu__ = this->async_TapirAccept(cmd_id, ballot, decision);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_TapirFastAccept(const uint64_t& cmd_id, const std::vector<SimpleCommand>& txn_cmds, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::TAPIRFASTACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd_id;
            *__cl__ << txn_cmds;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 TapirFastAccept(const uint64_t& cmd_id, const std::vector<SimpleCommand>& txn_cmds, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_TapirFastAccept(cmd_id, txn_cmds);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_TapirDecide(const uint64_t& cmd_id, const rrr::i32& commit, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::TAPIRDECIDE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd_id;
            *__cl__ << commit;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 TapirDecide(const uint64_t& cmd_id, const rrr::i32& commit) {
        rrr::Future* __fu__ = this->async_TapirDecide(cmd_id, commit);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_CarouselReadAndPrepare(const rrr::i64& tid, const MarshallDeputy& cmd, const bool_t& leader, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::CAROUSELREADANDPREPARE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
            *__cl__ << cmd;
            *__cl__ << leader;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CarouselReadAndPrepare(const rrr::i64& tid, const MarshallDeputy& cmd, const bool_t& leader, rrr::i32* res, TxnOutput* output) {
        rrr::Future* __fu__ = this->async_CarouselReadAndPrepare(tid, cmd, leader);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_CarouselAccept(const uint64_t& cmd_id, const int64_t& ballot, const int32_t& decision, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::CAROUSELACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd_id;
            *__cl__ << ballot;
            *__cl__ << decision;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CarouselAccept(const uint64_t& cmd_id, const int64_t& ballot, const int32_t& decision) {
        rrr::Future* __fu__ = this->async_CarouselAccept(cmd_id, ballot, decision);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_CarouselFastAccept(const uint64_t& cmd_id, const std::vector<SimpleCommand>& txn_cmds, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::CAROUSELFASTACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd_id;
            *__cl__ << txn_cmds;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CarouselFastAccept(const uint64_t& cmd_id, const std::vector<SimpleCommand>& txn_cmds, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_CarouselFastAccept(cmd_id, txn_cmds);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_CarouselDecide(const uint64_t& cmd_id, const rrr::i32& commit, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::CAROUSELDECIDE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd_id;
            *__cl__ << commit;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CarouselDecide(const uint64_t& cmd_id, const rrr::i32& commit) {
        rrr::Future* __fu__ = this->async_CarouselDecide(cmd_id, commit);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccDispatch(const std::vector<SimpleCommand>& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCDISPATCH, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccDispatch(const std::vector<SimpleCommand>& cmd, rrr::i32* res, TxnOutput* output, MarshallDeputy* md_graph) {
        rrr::Future* __fu__ = this->async_RccDispatch(cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
            __fu__->get_reply() >> *md_graph;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccFinish(const cmdid_t& id, const MarshallDeputy& md_graph, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCFINISH, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << md_graph;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccFinish(const cmdid_t& id, const MarshallDeputy& md_graph, std::map<uint32_t, std::map<int32_t, Value>>* outputs) {
        rrr::Future* __fu__ = this->async_RccFinish(id, md_graph);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *outputs;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccInquire(const txnid_t& txn_id, const int32_t& rank, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCINQUIRE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << txn_id;
            *__cl__ << rank;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccInquire(const txnid_t& txn_id, const int32_t& rank, std::map<uint64_t, parent_set_t>* out_0) {
        rrr::Future* __fu__ = this->async_RccInquire(txn_id, rank);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *out_0;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccDispatchRo(const SimpleCommand& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCDISPATCHRO, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccDispatchRo(const SimpleCommand& cmd, std::map<rrr::i32, Value>* output) {
        rrr::Future* __fu__ = this->async_RccDispatchRo(cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccInquireValidation(const txid_t& tx_id, const int32_t& rank, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCINQUIREVALIDATION, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tx_id;
            *__cl__ << rank;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccInquireValidation(const txid_t& tx_id, const int32_t& rank, int32_t* res) {
        rrr::Future* __fu__ = this->async_RccInquireValidation(tx_id, rank);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccNotifyGlobalValidation(const txid_t& tx_id, const int32_t& rank, const int32_t& res, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCNOTIFYGLOBALVALIDATION, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tx_id;
            *__cl__ << rank;
            *__cl__ << res;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccNotifyGlobalValidation(const txid_t& tx_id, const int32_t& rank, const int32_t& res) {
        rrr::Future* __fu__ = this->async_RccNotifyGlobalValidation(tx_id, rank, res);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_JanusDispatch(const std::vector<SimpleCommand>& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::JANUSDISPATCH, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 JanusDispatch(const std::vector<SimpleCommand>& cmd, rrr::i32* res, TxnOutput* output, MarshallDeputy* ret_graph) {
        rrr::Future* __fu__ = this->async_JanusDispatch(cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
            __fu__->get_reply() >> *ret_graph;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccCommit(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, const parent_set_t& parents, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCCOMMIT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << rank;
            *__cl__ << need_validation;
            *__cl__ << parents;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccCommit(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, const parent_set_t& parents, int32_t* res, TxnOutput* output) {
        rrr::Future* __fu__ = this->async_RccCommit(id, rank, need_validation, parents);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_JanusCommit(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, const MarshallDeputy& graph, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::JANUSCOMMIT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << rank;
            *__cl__ << need_validation;
            *__cl__ << graph;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 JanusCommit(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, const MarshallDeputy& graph, int32_t* res, TxnOutput* output) {
        rrr::Future* __fu__ = this->async_JanusCommit(id, rank, need_validation, graph);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_JanusCommitWoGraph(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::JANUSCOMMITWOGRAPH, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << id;
            *__cl__ << rank;
            *__cl__ << need_validation;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 JanusCommitWoGraph(const cmdid_t& id, const rank_t& rank, const int32_t& need_validation, int32_t* res, TxnOutput* output) {
        rrr::Future* __fu__ = this->async_JanusCommitWoGraph(id, rank, need_validation);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_JanusInquire(const epoch_t& epoch, const txnid_t& txn_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::JANUSINQUIRE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << epoch;
            *__cl__ << txn_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 JanusInquire(const epoch_t& epoch, const txnid_t& txn_id, MarshallDeputy* ret_graph) {
        rrr::Future* __fu__ = this->async_JanusInquire(epoch, txn_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *ret_graph;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccPreAccept(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCPREACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << txn_id;
            *__cl__ << rank;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccPreAccept(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, rrr::i32* res, parent_set_t* x) {
        rrr::Future* __fu__ = this->async_RccPreAccept(txn_id, rank, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *x;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_JanusPreAccept(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, const MarshallDeputy& graph, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::JANUSPREACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << txn_id;
            *__cl__ << rank;
            *__cl__ << cmd;
            *__cl__ << graph;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 JanusPreAccept(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, const MarshallDeputy& graph, rrr::i32* res, MarshallDeputy* ret_graph) {
        rrr::Future* __fu__ = this->async_JanusPreAccept(txn_id, rank, cmd, graph);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *ret_graph;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_JanusPreAcceptWoGraph(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::JANUSPREACCEPTWOGRAPH, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << txn_id;
            *__cl__ << rank;
            *__cl__ << cmd;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 JanusPreAcceptWoGraph(const cmdid_t& txn_id, const rank_t& rank, const std::vector<SimpleCommand>& cmd, rrr::i32* res, MarshallDeputy* ret_graph) {
        rrr::Future* __fu__ = this->async_JanusPreAcceptWoGraph(txn_id, rank, cmd);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *ret_graph;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_RccAccept(const cmdid_t& txn_id, const rrr::i32& rank, const ballot_t& ballot, const parent_set_t& p, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::RCCACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << txn_id;
            *__cl__ << rank;
            *__cl__ << ballot;
            *__cl__ << p;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 RccAccept(const cmdid_t& txn_id, const rrr::i32& rank, const ballot_t& ballot, const parent_set_t& p, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_RccAccept(txn_id, rank, ballot, p);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_JanusAccept(const cmdid_t& txn_id, const rrr::i32& rank, const ballot_t& ballot, const MarshallDeputy& graph, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::JANUSACCEPT, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << txn_id;
            *__cl__ << rank;
            *__cl__ << ballot;
            *__cl__ << graph;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 JanusAccept(const cmdid_t& txn_id, const rrr::i32& rank, const ballot_t& ballot, const MarshallDeputy& graph, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_JanusAccept(txn_id, rank, ballot, graph);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_PreAcceptFebruus(const txid_t& tx_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::PREACCEPTFEBRUUS, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tx_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 PreAcceptFebruus(const txid_t& tx_id, rrr::i32* ret, uint64_t* timestamp) {
        rrr::Future* __fu__ = this->async_PreAcceptFebruus(tx_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *ret;
            __fu__->get_reply() >> *timestamp;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_AcceptFebruus(const txid_t& tx_id, const ballot_t& ballot, const uint64_t& timestamp, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::ACCEPTFEBRUUS, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tx_id;
            *__cl__ << ballot;
            *__cl__ << timestamp;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 AcceptFebruus(const txid_t& tx_id, const ballot_t& ballot, const uint64_t& timestamp, rrr::i32* ret) {
        rrr::Future* __fu__ = this->async_AcceptFebruus(tx_id, ballot, timestamp);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *ret;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_CommitFebruus(const txid_t& tx_id, const uint64_t& timestamp, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClassicService::COMMITFEBRUUS, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tx_id;
            *__cl__ << timestamp;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 CommitFebruus(const txid_t& tx_id, const uint64_t& timestamp, rrr::i32* ret) {
        rrr::Future* __fu__ = this->async_CommitFebruus(tx_id, timestamp);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *ret;
        }
        __fu__->release();
        return __ret__;
    }
};

class ServerControlService: public rrr::Service {
public:
    enum {
        SERVER_SHUTDOWN = 0x35bda1cb,
        SERVER_READY = 0x5e2b93a7,
        SERVER_HEART_BEAT_WITH_DATA = 0x30c425e1,
        SERVER_HEART_BEAT = 0x13f807da,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(SERVER_SHUTDOWN, this, &ServerControlService::__server_shutdown__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(SERVER_READY, this, &ServerControlService::__server_ready__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(SERVER_HEART_BEAT_WITH_DATA, this, &ServerControlService::__server_heart_beat_with_data__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(SERVER_HEART_BEAT, this, &ServerControlService::__server_heart_beat__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(SERVER_SHUTDOWN);
        svr->unreg(SERVER_READY);
        svr->unreg(SERVER_HEART_BEAT_WITH_DATA);
        svr->unreg(SERVER_HEART_BEAT);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void server_shutdown(rrr::DeferredReply* defer) = 0;
    virtual void server_ready(rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void server_heart_beat_with_data(ServerResponse* res, rrr::DeferredReply* defer) = 0;
    virtual void server_heart_beat(rrr::DeferredReply* defer) = 0;
private:
    void __server_shutdown__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->server_shutdown(__defer__);
    }
    void __server_ready__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->server_ready(out_0, __defer__);
    }
    void __server_heart_beat_with_data__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        ServerResponse* out_0 = new ServerResponse;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->server_heart_beat_with_data(out_0, __defer__);
    }
    void __server_heart_beat__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->server_heart_beat(__defer__);
    }
};

class ServerControlProxy {
protected:
    rrr::Client* __cl__;
public:
    ServerControlProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_server_shutdown(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ServerControlService::SERVER_SHUTDOWN, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 server_shutdown() {
        rrr::Future* __fu__ = this->async_server_shutdown();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_server_ready(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ServerControlService::SERVER_READY, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 server_ready(rrr::i32* res) {
        rrr::Future* __fu__ = this->async_server_ready();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_server_heart_beat_with_data(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ServerControlService::SERVER_HEART_BEAT_WITH_DATA, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 server_heart_beat_with_data(ServerResponse* res) {
        rrr::Future* __fu__ = this->async_server_heart_beat_with_data();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_server_heart_beat(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ServerControlService::SERVER_HEART_BEAT, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 server_heart_beat() {
        rrr::Future* __fu__ = this->async_server_heart_beat();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

class ClientControlService: public rrr::Service {
public:
    enum {
        CLIENT_GET_TXN_NAMES = 0x67c42812,
        CLIENT_SHUTDOWN = 0x30c378c4,
        CLIENT_FORCE_STOP = 0x1ce017a0,
        CLIENT_RESPONSE = 0x46f447c2,
        CLIENT_READY = 0x2431f000,
        CLIENT_READY_BLOCK = 0x53265227,
        CLIENT_START = 0x3a05aff3,
        DISPATCHTXN = 0x14d98f9d,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(CLIENT_GET_TXN_NAMES, this, &ClientControlService::__client_get_txn_names__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_SHUTDOWN, this, &ClientControlService::__client_shutdown__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_FORCE_STOP, this, &ClientControlService::__client_force_stop__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_RESPONSE, this, &ClientControlService::__client_response__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_READY, this, &ClientControlService::__client_ready__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_READY_BLOCK, this, &ClientControlService::__client_ready_block__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_START, this, &ClientControlService::__client_start__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(DISPATCHTXN, this, &ClientControlService::__DispatchTxn__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(CLIENT_GET_TXN_NAMES);
        svr->unreg(CLIENT_SHUTDOWN);
        svr->unreg(CLIENT_FORCE_STOP);
        svr->unreg(CLIENT_RESPONSE);
        svr->unreg(CLIENT_READY);
        svr->unreg(CLIENT_READY_BLOCK);
        svr->unreg(CLIENT_START);
        svr->unreg(DISPATCHTXN);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void client_get_txn_names(std::map<rrr::i32, std::string>* txn_names, rrr::DeferredReply* defer) = 0;
    virtual void client_shutdown(rrr::DeferredReply* defer) = 0;
    virtual void client_force_stop(rrr::DeferredReply* defer) = 0;
    virtual void client_response(const DepId& dep_id, ClientResponse* res, rrr::DeferredReply* defer) = 0;
    virtual void client_ready(rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void client_ready_block(rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void client_start(rrr::DeferredReply* defer) = 0;
    virtual void DispatchTxn(const TxDispatchRequest& req, TxReply* result, rrr::DeferredReply* defer) = 0;
private:
    void __client_get_txn_names__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::map<rrr::i32, std::string>* out_0 = new std::map<rrr::i32, std::string>;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->client_get_txn_names(out_0, __defer__);
    }
    void __client_shutdown__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->client_shutdown(__defer__);
    }
    void __client_force_stop__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->client_force_stop(__defer__);
    }
    void __client_response__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        DepId* in_0 = new DepId;
        req->m >> *in_0;
        ClientResponse* out_0 = new ClientResponse;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->client_response(*in_0, out_0, __defer__);
    }
    void __client_ready__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->client_ready(out_0, __defer__);
    }
    void __client_ready_block__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->client_ready_block(out_0, __defer__);
    }
    void __client_start__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->client_start(__defer__);
    }
    void __DispatchTxn__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        TxDispatchRequest* in_0 = new TxDispatchRequest;
        req->m >> *in_0;
        TxReply* out_0 = new TxReply;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->DispatchTxn(*in_0, out_0, __defer__);
    }
};

class ClientControlProxy {
protected:
    rrr::Client* __cl__;
public:
    ClientControlProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_client_get_txn_names(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_GET_TXN_NAMES, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_get_txn_names(std::map<rrr::i32, std::string>* txn_names) {
        rrr::Future* __fu__ = this->async_client_get_txn_names();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *txn_names;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_shutdown(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_SHUTDOWN, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_shutdown() {
        rrr::Future* __fu__ = this->async_client_shutdown();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_force_stop(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_FORCE_STOP, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_force_stop() {
        rrr::Future* __fu__ = this->async_client_force_stop();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_response(const DepId& dep_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_RESPONSE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << dep_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_response(const DepId& dep_id, ClientResponse* res) {
        rrr::Future* __fu__ = this->async_client_response(dep_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_ready(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_READY, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_ready(rrr::i32* res) {
        rrr::Future* __fu__ = this->async_client_ready();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_ready_block(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_READY_BLOCK, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_ready_block(rrr::i32* res) {
        rrr::Future* __fu__ = this->async_client_ready_block();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_start(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_START, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_start() {
        rrr::Future* __fu__ = this->async_client_start();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_DispatchTxn(const TxDispatchRequest& req, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::DISPATCHTXN, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << req;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 DispatchTxn(const TxDispatchRequest& req, TxReply* result) {
        rrr::Future* __fu__ = this->async_DispatchTxn(req);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *result;
        }
        __fu__->release();
        return __ret__;
    }
};

} // namespace janus



