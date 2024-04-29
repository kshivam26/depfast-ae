#pragma once

#include <math.h>
#include "__dep__.h"
#include "rrr.hpp"
#include "constants.h"
#include "command.h"
#include "deptran/procedure.h"
#include "command_marshaler.h"
#include "rcc/graph.h"
#include "rcc/graph_marshaler.h"
// #include "benchmark_service.h"
#include "rcc_rpc.h"

// using namespace benchmark;
class SimpleCommand;
// using namespace rrr;

namespace janus {
static Counter g_nop_counter;


void BenchmarkService::nop(const std::string&, rrr::DeferredReply* defer) {
    int cnt = g_nop_counter.next();
    if (cnt % 2000000 == 0) {
        Log_info("%d nop requests with tid: %d", cnt, gettid());
    }
    defer->reply();
}
}