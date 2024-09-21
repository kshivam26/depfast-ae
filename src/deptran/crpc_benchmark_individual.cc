#include "__dep__.h"
#include <string>
#include <vector>
#include <algorithm>
#include <atomic>

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include "constants.h"
#include "command.h"
#include "deptran/procedure.h"
#include "command_marshaler.h"
#include "rcc/graph.h"
#include "rcc/graph_marshaler.h"
#include "rcc_rpc.h"
// #include "rrr.hpp"
// #include "benchmark_service.h"
// #include "crpc_service.h"
// #include "crpc_service.h"
#include "crpc_service.cc"

// using namespace crpc;
// using namespace rcc;
using namespace rrr;
using namespace std;
using namespace janus;

const char *svr_addr = "127.0.0.1:13001";
std::vector<const char*> svr_addrs = {};
int batch_size = 100;
int epoll_instances = 1;
bool fast_requests = false;
int seconds = 10;
int client_threads = 8;
int current_index = 1;
int worker_threads = 16;
int payloadSize = 1;
int outgoing_requests = 1000;
int number_servers = 1;
static string request_str;
PollMgr *poll = nullptr;
// PollMgr **polls;
ThreadPool* thrpool = nullptr;

Counter req_counter;
std::vector<std::vector<double> > *response;
std::atomic<uint32_t> response_index;
std::vector<uint64_t> qps;

bool should_stop = false;

pthread_mutex_t g_stop_mutex;
pthread_cond_t g_stop_cond;

static void signal_handler(int sig) {
    Log_info("caught signal %d, stopping server now", sig);
    should_stop = true;
    Pthread_mutex_lock(&g_stop_mutex);
    Pthread_cond_signal(&g_stop_cond);
    Pthread_mutex_unlock(&g_stop_mutex);
}

// static void* stat_proc(void*) {
//     i64 last_cnt = 0;
//     response_index.store(0);
//     for (int i = 0; i < seconds; i++) {
//         int cnt = req_counter.peek_next();
//         // Log_info("current cnt: %d and last_cnt: %d", cnt, last_cnt);
//         response_index++;
//         if (last_cnt != 0) {
//             qps[i] = cnt - last_cnt;
//             Log_info("qps: %ld", cnt - last_cnt);
//         } else {
//             qps[i] = 0;
//         }
//         last_cnt = cnt;
//         sleep(1);
//     }
//     should_stop = true;

//     int max_index = 0;
//     uint64_t max_qps = 0;
//     int start = seconds/4;
//     int end = seconds*3/4;

//     double sumI = 0.0;
//     for (int k = start; k <= end; ++k) {
//         sumI += qps[k];
//     }

//     // Log_info("end: %d, and start: %d", end, start);
//     auto avgQps = sumI / (end - start + 1);

//     for (int i = seconds / 3; i < seconds * 2 / 3; i++) {
//         if (qps[i] > max_qps) {
//             max_index = i;
//             max_qps = qps[i];
//         }
//     }

//     // auto avg_qps = req_counter.peek_next()/seconds;
//     // printf("Avg QPS: %lf;\n", avgQps);


//     std::vector<double> latency;

//     for (int i = 0; i < client_threads; i++){
//         for (int k = start; k <= end; ++k) {
//             latency.insert(latency.end(), response[i][k].begin(), response[i][k].end());
//         }
//     }

//     // for (int i = 0; i < client_threads; i++)
//     //     latency.insert(latency.end(), response[i][max_index].begin(), response[i][max_index].end());
//     std::sort(latency.begin(), latency.end());
//     size_t l_length = latency.size();
//     double sum = 0.0;
//     int p50 = floor(l_length * 0.5);
//     int p90 = floor(l_length * 0.90);
//     int p99 = floor(l_length * 0.99);
//     int p999 = floor(l_length * 0.999);
//     double l50, l90, l99, l999;
//     int ii = 0;
//     for (; ii < p50; ii++)
//         sum += latency[ii];
//     l50 = sum / p50;
//     for (; ii < p90; ii++)
//         sum += latency[ii];
//     l90 = sum / p90;
//     for (; ii < p99; ii++)
//         sum += latency[ii];
//     l99 = sum / p99;
//     for (; ii < p999; ii++)
//         sum += latency[ii];
//     l999 = sum / p999;
//     printf("QPS: %lf; 50.0%% LATENCY: %lf; 90.0%% LATENCY: %lf; 99.0%% LATENCY: %lf; 99.9%% LATENCY: %lf\n", avgQps, l50 * 1000, l90 * 1000, l99 * 1000, l999 * 1000);
//     pthread_exit(nullptr);
//     return nullptr;
// }

// typedef struct {
//     uint32_t cid;
// } client_para_t;

// static void* client_proc(void *args) {
//     client_para_t *client_para = (client_para_t *)args;
//     uint32_t cid = client_para->cid;
//     polls[cid] = new PollMgr(1);
//     auto cl = std::make_shared<rrr::Client>(polls[cid]);
//     verify(cl->connect(svr_addrs[0]) == 0);
//     i32 rpc_id;
//     if (fast_requests) {
//         Log_info("Running crpc client");
//         rpc_id = SampleCrpcServiceImpl::CRPC_NOP;
//     } else {
//         Log_info("Running Broadcast client");
//         rpc_id = SampleCrpcServiceImpl::NOP;
//     }
//     struct timespec last_time;
//     //base::Timer timer;
//     FutureAttr fu_attr;
//     auto do_work = [cl, &fu_attr, rpc_id, &last_time] {
//         if (!should_stop) {
//             clock_gettime(CLOCK_REALTIME, &last_time);
//             Future* fu = cl->begin_request(rpc_id, fu_attr);
//             *cl << request_str;
//             cl->end_request();
//             Future::safe_release(fu);
//             req_counter.next();
//         }
//     };
//     fu_attr.callback = [&do_work, &last_time, cid] (Future* fu) {
//         if (fu->get_error_code() != 0) {
//             return;
//         }
//         struct timespec t_buf;
//         clock_gettime(CLOCK_REALTIME, &t_buf);
//         //timer.stop();
//         response[cid][response_index.load()].push_back(t_buf.tv_sec - last_time.tv_sec + (t_buf.tv_nsec - last_time.tv_nsec) / 1000000000.0);
//         last_time = t_buf;
//         // Log_info("response received");
//         do_work(); // ks: uncomment later
//     };
//     for (int i = 0; i < outgoing_requests; i++) {
//         // Log_info("client cp3:");
//         do_work();
//         // Log_info("client cp4:");
//     }
//     while (!should_stop) {
//         sleep(1);
//         // std::cout << "should_stop still false";
//     }

//     // std::cout << "After should_stop ended\n";

//     cl->close_and_release();
//     polls[cid]->release();
//     pthread_exit(nullptr);
//     return nullptr;
// }

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    bool is_client = false, is_server = false;

    if (argc < 2) {
        printf("usage: perftest OPTIONS\n");
        printf("                -c|-s ip:port\n");
        printf("                -b    byte_size\n");        
        printf("                -f    fast_requests\n");
        printf("                -n    seconds\n");
        printf("                -o    outgoing_requests\n");
        printf("                -t    currentIndex\n");
        printf("                -w    payload_size\n");
        printf("                -e    epoll_instances\n");
        exit(1);
    }

    char ch = 0;
    while ((ch = getopt(argc, argv, "c:s:b:e:f:n:o:t:w:x:"))!= -1) {
        Log_info("current character arg: %c", ch);
        switch (ch) {
        // case 'c':
        //     Log_info("inside is client");
        //     is_client = true;
        //     svr_addr = optarg;
        //     break;
        // case 's':
        //     is_server = true;
        //     svr_addr = optarg;
        //     break;
        case 'b':
            batch_size = atoi(optarg);
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
            current_index = atoi(optarg);
            break;
        case 'w':
            payloadSize =  atoi(optarg);
            break;
        case 'x':
            number_servers = atoi(optarg);
        default:
            break;
        }
    }
    // verify(is_server || is_client);
    // if (is_server) {
    //     Log_info("server will start at     %s", svr_addr);
    // } else {
    //     Log_info("client will connect to   %s", svr_addr);
    // }
    Log_info("batch size:        %d", batch_size);
    Log_info("epoll instances:         %d", epoll_instances);
    Log_info("fast reqeust:            %s", fast_requests ? "true" : "false");
    Log_info("running seconds:         %d", seconds);
    Log_info("outgoing requests:       %d", outgoing_requests);
    Log_info("client threads:          %d", current_index);
    Log_info("payloadSize:          %d", payloadSize);
    Log_info("number of servers:          %d", number_servers);

    if (number_servers > 0) {
        for (int i = 1; i <= number_servers; ++i) {
            // Constructing the address string
            std::string address = "172.19.0." + std::to_string(i+10) + ":2100"; // 172.19.0.11 ...

            // Converting the string to a const char* and adding it to the vector
            // Note: Be aware that the temporary string will be destructed after the iteration.
            // For a more robust solution, consider storing the string itself or using dynamic allocation.
            svr_addrs.push_back(strdup(address.c_str()));
            if (i == current_index){
                svr_addr = strdup(address.c_str());
            }
        }
    }

    // request_str = string(1, 'x');
    // thrpool = new ThreadPool(worker_threads); // ks: delete this 
    size_t numServers = svr_addrs.size();

    Log_info("Number of servers2: %d",numServers);
    // Initialize vector of Server pointers with nullptr
    // std::vector<Server*> serverVector(numServers, nullptr);
    is_server = true;
    if (is_server) {
        // vector<PollMgr*> polls(numServers, nullptr);
        // vector<ThreadPool*> thrpools(numServers, nullptr); // delete this
        // vector<SampleCrpcServiceImpl*> BS(numServers, nullptr);;
        // for (int i=0; i < svr_addrs.size(); i++){
        //     poll = new PollMgr(epoll_instances);
        //     // thrpools[i] = new ThreadPool(worker_threads);
        //     thrpool = nullptr;
        //     SampleCrpcServiceImpl* BS  = new SampleCrpcServiceImpl(svr_addr, svr_addrs, poll);
        //     auto server = new Server(poll, nullptr);
        //     server->reg(BS);
        //     verify(server->start(svr_addr) == 0);
        // }

        poll = new PollMgr(epoll_instances);
            // thrpools[i] = new ThreadPool(worker_threads);
        thrpool = nullptr;
        SampleCrpcServiceImpl* BS  = new SampleCrpcServiceImpl(svr_addr, svr_addrs, poll);
        auto server = new Server(poll, nullptr);
        server->reg(BS);
        verify(server->start(svr_addr) == 0);

        sleep(2);

        Log_info("starting the setup on the servers number: %s", svr_addr);
        BS->setup(payloadSize);

        sleep(1);

        if (current_index == 1)
            BS->start(outgoing_requests, seconds, fast_requests, batch_size);

        Pthread_mutex_init(&g_stop_mutex, nullptr);
        Pthread_cond_init(&g_stop_cond, nullptr);

        signal(SIGPIPE, SIG_IGN);
        signal(SIGHUP, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        signal(SIGALRM, signal_handler);
        signal(SIGINT, signal_handler);
        signal(SIGQUIT, signal_handler);
        signal(SIGTERM, signal_handler);

        // struct sigaction sa;
        // sa.sa_handler = signal_handler;
        // sigemptyset(&sa.sa_mask);
        // sa.sa_flags = SA_RESTART;  // Restart system calls if possible

        // sigaction(SIGALRM, &sa, NULL);
        // sigaction(SIGINT, &sa, NULL);
        // sigaction(SIGQUIT, &sa, NULL);
        // sigaction(SIGTERM, &sa, NULL);

        // sigset_t mask;
        // sigfillset(&mask);
        // sigdelset(&mask, SIGALRM);
        // sigdelset(&mask, SIGINT);
        // sigdelset(&mask, SIGQUIT);
        // sigdelset(&mask, SIGTERM);
        // pthread_sigmask(SIG_SETMASK, &mask, NULL);

        Pthread_mutex_lock(&g_stop_mutex);
        while (should_stop == false) {
            Pthread_cond_wait(&g_stop_cond, &g_stop_mutex);
        }
        Pthread_mutex_unlock(&g_stop_mutex);
        std::cout << "Shutting down gracefully...1\n";
        // for (int i=0; i < svr_addrs.size(); i++){            
        //     // thrpools[i] = new ThreadPool(worker_threads);
        //     std::cout << "Shutting down service...1\n";
        //     delete BS[i];
        //     std::cout << "Shutting down server...1\n";
        //     delete serverVector[i];
        //     std::cout << "Shutting down poll...1\n";
        //     polls[i]->release();
        //     std::cout << "release successful\n";
        // }
        poll->release();
        delete BS;
        delete server;
        std::cout << "Shutting down gracefully...\n";
        // pthread_mutex_destroy(&g_stop_mutex);
        // pthread_cond_destroy(&g_stop_cond);

    } else {
        verify(0);
        // response = new std::vector<std::vector<double> >[client_threads];
        // qps.resize(seconds);
        // polls = (PollMgr **)malloc(sizeof(PollMgr *) * client_threads);
        // pthread_t* client_th = new pthread_t[client_threads];
        // client_para_t *client_para = (client_para_t *)malloc(client_threads * sizeof(client_para_t));
        // for (int i = 0; i < client_threads; i++) {
        //     response[i].resize(seconds + 1);
        //     client_para[i].cid = i;
        //     Pthread_create(&client_th[i], nullptr, client_proc, (void *)(client_para + i));
        // }
        // pthread_t stat_th;
        // Pthread_create(&stat_th, nullptr, stat_proc, nullptr);
        // Pthread_join(stat_th, nullptr);
        // for (int i = 0; i < client_threads; i++) {
        //     Pthread_join(client_th[i], nullptr);
        // }

        // std::cout << "After client thread ended\n";
        // delete [] response;
        // free(polls);
        // delete[] client_th;
    }

    thrpool->release();
    return 0;
}
