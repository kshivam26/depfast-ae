#include <string>
#include "__dep__.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "constants.h"
#include "command.h"
#include "deptran/procedure.h"
#include "command_marshaler.h"
#include "rcc/graph.h"
#include "rcc/graph_marshaler.h"
#include "rcc_rpc.h"

class SimpleCommand;
// using namespace benchmark;
using namespace rrr;
using namespace std;
using namespace janus;

const char *svr_addr = "127.0.0.1:8848";
int byte_size = 10;
int epoll_instances = 2;
bool fast_requests = false;
int seconds = 10;
int outgoing_requests = 1000;
int client_threads = 8;
int worker_threads = 16;

static string request_str;
PollMgr* poll;
ThreadPool* thrpool;

Counter req_counter;

bool should_stop = false;

pthread_mutex_t g_stop_mutex;
pthread_cond_t g_stop_cond;
std::vector<uint64_t> qps;

static void signal_handler(int sig) {
    Log_info("caught signal %d, stopping server now", sig);
    should_stop = true;
    Pthread_mutex_lock(&g_stop_mutex);
    Pthread_cond_signal(&g_stop_cond);
    Pthread_mutex_unlock(&g_stop_mutex);
}

static void* stat_proc(void*) {
    i64 last_cnt = 0;
    for (int i = 0; i < seconds; i++) {
        int cnt = req_counter.peek_next();
        if (last_cnt != 0) {
            qps[i] = cnt - last_cnt;
            Log_info("qps: %ld", cnt - last_cnt);
        }
        last_cnt = cnt;
        sleep(1);
    }
    should_stop = true;

    int start = seconds/4;
    int end = seconds*3/4;
    int x = 0;
    double sumI = 0.0;
    for (int k = start; k <= end; ++k) {
        x++;
        sumI += qps[k];
    }

    Log_info("end: %d, and start: %d, x:%d", end, start, x);
    auto avgQps = sumI / (end - start + 1);

    printf("Avg QPS: %lf;\n", avgQps);
    Log_info("Average qps: %ld", req_counter.peek_next()/seconds);
    pthread_exit(nullptr);
    return nullptr;
}

static void* client_proc(void*) {
    // Log_info("client_proc; cp-1");
    auto cl = std::make_shared<rrr::Client>(poll);
    // Log_info("client_proc; cp0");
    verify(cl->connect(svr_addr) == 0);
    i32 rpc_id;
    // if (fast_requests) {
    //     rpc_id = BenchmarkService::FAST_NOP;
    // } else {
    //     rpc_id = BenchmarkService::NOP;
    // }
    // Log_info("client_proc; cp1");
    rpc_id = BenchmarkService::NOP;
    FutureAttr fu_attr;
    auto do_work = [cl, &fu_attr, rpc_id] {
        if (!should_stop) {
            Future* fu = cl->begin_request(rpc_id, fu_attr);
            *cl << request_str;
            cl->end_request();
            Future::safe_release(fu);
            req_counter.next();
        }
    };
    fu_attr.callback = [&do_work] (Future* fu) {
        if (fu->get_error_code() != 0) {
            return;
        }
        //thrpool->run_async([&do_work] {
            do_work();
        //});
    };
    // Log_info("client_proc; cp2");
    for (int i = 0; i < outgoing_requests; i++) {
        do_work();
    }
    
    while (!should_stop) {
        sleep(1);
    }
    // Log_info("client_proc; cp3");
    cl->close_and_release();
    pthread_exit(nullptr);
    return nullptr;
}

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    bool is_client = false, is_server = false;

    if (argc < 2) {
        printf("usage: perftest OPTIONS\n");
        printf("                -c|-s ip:port\n");
        printf("                -b    byte_size\n");
        printf("                -e    epoll_instances\n");
        printf("                -f    fast_requests\n");
        printf("                -n    seconds\n");
        printf("                -o    outgoing_requests\n");
        printf("                -t    client_threads\n");
        printf("                -w    worker_threads\n");
        exit(1);
    }

    char ch = 0;
    while ((ch = getopt(argc, argv, "c:s:b:e:fn:o:t:w:"))!= -1) {
        switch (ch) {
        case 'c':
            is_client = true;
            svr_addr = optarg;
            break;
        case 's':
            is_server = true;
            svr_addr = optarg;
            break;
        case 'b':
            byte_size = atoi(optarg);
            break;
        case 'e':
            epoll_instances = atoi(optarg);
            break;
        case 'f':
            fast_requests = true;
            break;
        case 'n':
            seconds = atoi(optarg);
            break;
        case 'o':
            outgoing_requests = atoi(optarg);
            break;
        case 't':
            client_threads = atoi(optarg);
            break;
        case 'w':
            worker_threads = atoi(optarg);
            break;
        default:
            break;
        }
    }
    verify(is_server || is_client);
    if (is_server) {
        Log_info("server will start at     %s", svr_addr);
    } else {
        Log_info("client will connect to   %s", svr_addr);
    }
    Log_info("packet byte size:        %d", byte_size);
    Log_info("epoll instances:         %d", epoll_instances);
    Log_info("fast reqeust:            %s", fast_requests ? "true" : "false");
    Log_info("running seconds:         %d", seconds);
    Log_info("outgoing requests:       %d", outgoing_requests);
    Log_info("client threads:          %d", client_threads);
    Log_info("worker threads:          %d", worker_threads);

    request_str = string(byte_size, 'x');
    poll = new PollMgr(epoll_instances);
    
    if (is_server) {
        BenchmarkService svc;
        thrpool = new ThreadPool(worker_threads);
        Server svr(poll, thrpool);
        svr.reg(&svc);
        verify(svr.start(svr_addr) == 0);

        Pthread_mutex_init(&g_stop_mutex, nullptr);
        Pthread_cond_init(&g_stop_cond, nullptr);

        signal(SIGPIPE, SIG_IGN);
        signal(SIGHUP, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        signal(SIGALRM, signal_handler);
        signal(SIGINT, signal_handler);
        signal(SIGQUIT, signal_handler);
        signal(SIGTERM, signal_handler);

        Pthread_mutex_lock(&g_stop_mutex);
        while (should_stop == false) {
            Pthread_cond_wait(&g_stop_cond, &g_stop_mutex);
        }
        Pthread_mutex_unlock(&g_stop_mutex);

    } else {
        // Log_info("client start; cp1");
        qps.resize(seconds);
        pthread_t* client_th = new pthread_t[client_threads];
        for (int i = 0; i < client_threads; i++) {
            Pthread_create(&client_th[i], nullptr, client_proc, nullptr);
        }
        // Log_info("client start; cp2");
        pthread_t stat_th;
        Pthread_create(&stat_th, nullptr, stat_proc, nullptr);
        // Log_info("client start; cp3");
        Pthread_join(stat_th, nullptr);
        for (int i = 0; i < client_threads; i++) {
            // Log_info("client start; cp4");
            Pthread_join(client_th[i], nullptr);
        }
        // Log_info("client start; cp5");
        delete[] client_th;
    }

    poll->release();
    if (thrpool)
        thrpool->release();
    return 0;
}