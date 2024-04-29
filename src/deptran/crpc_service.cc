#pragma once
#include "__dep__.h"
#include "rrr.hpp"
// #include "../memdb/value.h"
// #include "../rcc_rpc.h"
#include "constants.h"
#include "command.h"
#include "deptran/procedure.h"
#include "command_marshaler.h"
#include "rcc/graph.h"
#include "rcc/graph_marshaler.h"
#include <atomic>
#include <numeric> 
#include <stdio.h>
#include "rcc_rpc.h"
// using namespace crpc;
// using namespace rrr;
class SimpleCommand;
namespace janus {

class SampleCrpcServiceImpl: public CrpcBenchmarkService{
    Counter nop_counter;
    Counter crpc_nop_counter;
    Counter crpc_num_done;
    Counter bcast_num_done;
    std::vector<const char*> svr_addrs_;
    std::map<uint16_t, CrpcBenchmarkProxy*> proxies;
    // PollMgr * poll = new PollMgr(1); // uncomment later
    PollMgr * prevPoll_;
    std::vector<PollMgr*> polls;
    std::vector<std::shared_ptr<rrr::Client>>clients;
    const char* self_addr_;
    std::mutex numMutex;
    std::map<int, int> response_counter;
    // std::map<int, int> response_counter;
    std::map<int, rrr::DeferredReply*> crpc_response_counter;
    std::map<int, struct timespec> response_saver;
    std::vector<double> latency_manager;
    std::vector<uint16_t> sitesInfo_;
    const size_t one_megabyte = 1048576; // Number of bytes in one megabyte
    const size_t ten_kilobyte = 10000; // Number of bytes in ten KB
    char character_to_repeat = 'a'; // Character to fill the string with
    int byte_size = 10;
    char chraacter_to_repeat = 'a'; // Character to fill the string with
    int cnt33, cnt66;
    bool isChain = false;
    // bool isChain = false;
    public:
        SampleCrpcServiceImpl(const char* self_addr, std::vector<const char*> svr_addrs, PollMgr * prevPoll):self_addr_(self_addr), svr_addrs_(svr_addrs), prevPoll_(prevPoll){
            // for (auto svr_addr : svr_addrs_) {
            //     Log_info("server address:  %s with tid:%d", svr_addr, gettid());
            // }
        }

        ~SampleCrpcServiceImpl(){
            // prevPoll_->release();
            // poll->release();
        }

        void start(int max_pending_requests, int seconds, bool isChain, int batchSize){
            Log_info("cp2; %d", byte_size);
            std::string payload(byte_size, character_to_repeat); 
            if (isChain){
                // something wrong with how oneTimeJob is executed. The previous coroutine is just 
                // scheduled out but then trigger job reruns the coroutine?
                // for (uint32_t n_tx = 0; n_tx < 10; n_tx++) {
                prevPoll_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>([this, batchSize, max_pending_requests, seconds, payload](){
                    Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                    Log_info("in Crpc; tid: %d", gettid());
                    auto beg_time = Time::now() ;
                    auto end_time = beg_time + seconds * pow(10, 6);
                    struct timespec start_;
                    start_.tv_sec = 0;
                    int last_sent = 0;
                    while(true){
                        // Log_info("Inside the loop");
                        auto cur_time = Time::now(); // optimize: this call is not scalable.
                        int cnt = crpc_nop_counter.peek_next();
                        int num_done = crpc_num_done.peek_next();
                        if (cur_time > end_time) {
                            Log_info("***** ENDING Chaining*****");
                            while (cnt !=num_done){
                                Log_info("#### Waiting for all the responses to return; cnt: %d, num_done: %d", cnt, num_done);
                                Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                                num_done = crpc_num_done.peek_next();
                            }
                            Log_info("throughput: %f", (double)(cnt66 - cnt33) / (seconds * 2.0 / 3 - seconds / 3.0));
                        
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

                        /*
                            chaining
                        */
                        while (true){
                            if (cnt-num_done >= max_pending_requests || last_sent >= batchSize){
                                // Log_info("pausing the one-time sending job; cnt: %d, num_done:%d", cnt, num_done);
                                Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 2));
                                // Log_info("woke up from wait");
                                last_sent = 0;
                                break;
                            }
                            cnt = crpc_nop_counter.next();
                            last_sent++;
                            // int q = proxies.size()/2+1;
                            if (cnt % 200000 == 0) {
                                Log_info("%d crpc nop requests", cnt);
                            }
                            struct timespec t_buf;
                            clock_gettime(CLOCK_REALTIME, &t_buf);
                            response_saver[cnt] = t_buf;
                            // state vector
                            std::vector<ResultAdd> state;


                            auto f = proxies[sitesInfo_[0]]->async_CrpcLargePayload(cnt, payload, sitesInfo_, state);
                            Future::safe_release(f);
                        }

                    }
                })));

                prevPoll_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>([this, max_pending_requests, seconds](){
                    Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                    auto beg_time = Time::now();
                    auto end_time1 = beg_time + (seconds/3) * pow(10, 6);
                    auto end_time2 = beg_time + (seconds*2/3) * pow(10,6);
                    struct timespec start_;
                    start_.tv_sec = 0;
                    int last_sent = 0;

                    while(true) {
                        auto cur_time = Time::now(); // optimize: this call is not scalable.
                        
                        if (cur_time > end_time1) {
                            cnt33 = crpc_num_done.peek_next();
                            Log_info("one third time done, cnt33: %d", cnt33);
                            break;
                        }
                        else {
                            Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                        }
                    }

                    while(true) {
                        auto cur_time = Time::now(); // optimize: this call is not scalable.
                        
                        if (cur_time > end_time2) {
                            cnt66 = crpc_num_done.peek_next();
                            Log_info("two third time done, cnt66: %d", cnt66);
                            break;
                        }
                        else {
                            Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                        }
                    }
                })));
                // }
            }
            else { // broadcast
                // for (uint32_t n_tx = 0; n_tx < 10; n_tx++) {
                prevPoll_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>([this, batchSize, max_pending_requests, seconds, payload](){
                Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                Log_info("in bcast; tid: %d", gettid());
                auto beg_time = Time::now() ;
                auto end_time = beg_time + seconds * pow(10, 6);
                struct timespec start_;
                start_.tv_sec = 0;
                int last_sent = 0;
                while(true){
                    // Log_info("Inside the loop");
                    auto cur_time = Time::now(); // optimize: this call is not scalable.
                    int cnt = nop_counter.peek_next();
                    int num_done = bcast_num_done.peek_next();
                    if (cur_time > end_time) {
                        Log_info("***** ENDING Bcast*****");
                        while (cnt !=num_done){
                            Log_info("#### Waiting for all the responses to return; cnt: %d, num_done: %d", cnt, num_done);
                            Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                            num_done = bcast_num_done.peek_next();                            
                        }
                        Log_info("throughput: %f", (double)(cnt66 - cnt33) / (seconds * 2.0 / 3 - seconds / 3.0));

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


                        ///////////////////////////
                        break;
                    }

                    while (true){
                        if (cnt-num_done >= max_pending_requests || last_sent >= batchSize){
                            // Log_info("pausing the one-time sending job; cnt: %d, num_done:%d", cnt, num_done);
                            Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 2));
                            // Log_info("woke up from wait");
                            last_sent = 0;
                            break;
                        }
                        last_sent++;
                        cnt = nop_counter.next();
                        // int q = proxies.size()/2+1;
                        if (cnt % 200000 == 0) {
                            Log_info("%d bcast nop requests", cnt);
                        }
                        int q = proxies.size()/2; // change add +1 back
                        auto numMutex2 = std::make_shared<std::mutex>();
                        struct timespec t_buf;
                        clock_gettime(CLOCK_REALTIME, &t_buf);
                        response_saver[cnt] = t_buf;
                        for (int i=1; i<proxies.size(); i++){
                            FutureAttr fu_attr;
                            // Log_info("#### inside loop, value of num: %d, and cnt: %d", num, cnt);
                            fu_attr.callback = [numMutex2, q, this, cnt] (Future* fu) {
                                std::lock_guard<std::mutex> lock(*numMutex2);
                                // Log_info("#### inside callback, value of cnt: %d", cnt);
                                if (fu->get_error_code() != 0) {
                                    Log_info("#### some ERROR, sending back response");
                                    // defer->reply();
                                    return;
                                }
                                this->response_counter[cnt] = this->response_counter[cnt]+1;
                                // Log_info("#### inside callback, value of response_counter: %ld", this->response_counter[cnt]);
                                if (this->response_counter[cnt] == q){
                                    auto it = response_saver.find(cnt);

                                    struct timespec t_buf;
                                    clock_gettime(CLOCK_REALTIME, &t_buf);
                                    auto last_time = it->second;
                                    latency_manager.push_back(t_buf.tv_sec - last_time.tv_sec + (t_buf.tv_nsec - last_time.tv_nsec) / 1000000000.0);
                                    response_saver.erase(it);
                                    // Log_info("#### sending reply");
                                    // Log_info("#### inside callback, value of response_counter: %ld", this->response_counter[cnt]);
                                    // defer->reply();
                                    int num = bcast_num_done.next();
                                    // Log_info("#### sending reply; id done: %d", num);
                                }
                                // Log_info("ending callback, value of cnt: %d", cnt);
                            };
                            auto f = proxies[i]->async_BroadcastLargePayload(cnt, payload, fu_attr);
                            Future::safe_release(f);
                            
                        }

                    }
                }
                })));

                prevPoll_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>([this, max_pending_requests, seconds](){
                    Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                    auto beg_time = Time::now();
                    auto end_time1 = beg_time + (seconds/3) * pow(10, 6);
                    auto end_time2 = beg_time + (seconds*2/3) * pow(10,6);
                    struct timespec start_;
                    start_.tv_sec = 0;
                    int last_sent = 0;

                    while(true) {
                        auto cur_time = Time::now(); // optimize: this call is not scalable.
                        
                        if (cur_time > end_time1) {
                            cnt33 = bcast_num_done.peek_next();
                            Log_info("one third time done, cnt33: %d", cnt33);
                            break;
                        }
                        else {
                            Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                        }
                    }

                    while(true) {
                        auto cur_time = Time::now(); // optimize: this call is not scalable.
                        
                        if (cur_time > end_time2) {
                            cnt66 = bcast_num_done.peek_next();
                            Log_info("two third time done, cnt66: %d", cnt66);
                            break;
                        }
                        else {
                            Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
                        }
                    }
                })));
                // }
            }

        }

        void setup(int payloadSize){
            // Log_info("server address:  %s with starting setup", self_addr_);
            uint16_t i = 0;
            if (payloadSize == 2){
                byte_size = one_megabyte;
            }
            else if (payloadSize == 3) {
                byte_size = ten_kilobyte;
            }

            // byte_size = isSmall ? 10 : one_megabyte;

            // Log_info("byte size is: %d", byte_size);
            
            // int duration = seconds;
            for (auto svr_addr : svr_addrs_) {
                // Log_info("server %s trying to connect to server:  %s", self_addr_, svr_addr);
                // auto poll = new PollMgr(1);
                auto rpc_cli = std::make_shared<rrr::Client>(prevPoll_);
                // Client * cl = new Client();
                auto result = rpc_cli->connect(svr_addr);
                if (result != 0){
                    Log_info("server %s failed to connect to server:  %s", self_addr_, svr_addr);
                }
                else {
                    // Log_info("server %s successfully connected to server:  %s", self_addr_, svr_addr);
                }
                clients.push_back(rpc_cli);
                auto proxy = new CrpcBenchmarkProxy(rpc_cli.get());
                proxies[i++] = proxy;
            }
            Log_info("cp1");
            sitesInfo_.resize(proxies.size()-1); // remove -1 later
            std::iota(sitesInfo_.begin(), sitesInfo_.end(), 1); // change it back to 0 later
            sitesInfo_.push_back(0);

            if (self_addr_ != svr_addrs_[0]){
                return;
            }

            
            // isChain = false;

            Log_info("starting job");
            // if (isChain){
            //     // something wrong with how oneTimeJob is executed. The previous coroutine is just 
            //     // scheduled out but then trigger job reruns the coroutine?
            //     // for (uint32_t n_tx = 0; n_tx < 10; n_tx++) {
            //     prevPoll_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>([this, batchSize, max_pending_requests, seconds, payload](){
            //         Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 7)*2);
            //         Log_info("in Crpc; tid: %d", gettid());
            //         auto beg_time = Time::now() ;
            //         auto end_time = beg_time + seconds * pow(10, 6);
            //         struct timespec start_;
            //         start_.tv_sec = 0;
            //         int last_sent = 0;
            //         while(true){
            //             // Log_info("Inside the loop");
            //             auto cur_time = Time::now(); // optimize: this call is not scalable.
            //             int cnt = crpc_nop_counter.peek_next();
            //             int num_done = crpc_num_done.peek_next();
            //             if (cur_time > end_time) {
            //                 Log_info("***** ENDING Chaining*****");
            //                 while (cnt !=num_done){
            //                     Log_info("#### Waiting for all the responses to return; cnt: %d, num_done: %d", cnt, num_done);
            //                     Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
            //                     num_done = crpc_num_done.peek_next();
            //                 }
            //                 Log_info("throughput: %f", (double)(cnt66 - cnt33) / (seconds * 2.0 / 3 - seconds / 3.0));
                        
            //                 ///////////////////////////
            //                 // latency calculation            

            //                 std::sort(latency_manager.begin(), latency_manager.end());
            //                 size_t l_length = latency_manager.size();
            //                 double sum = 0.0;
            //                 int p50 = floor(l_length * 0.5);
            //                 int p90 = floor(l_length * 0.90);
            //                 int p99 = floor(l_length * 0.99);
            //                 int p999 = floor(l_length * 0.999);
            //                 double l50, l90, l99, l999;
            //                 int ii = 0;
            //                 for (; ii < p50; ii++)
            //                     sum += latency_manager[ii];
            //                 l50 = sum / p50;
            //                 for (; ii < p90; ii++)
            //                     sum += latency_manager[ii];
            //                 l90 = sum / p90;
            //                 for (; ii < p99; ii++)
            //                     sum += latency_manager[ii];
            //                 l99 = sum / p99;
            //                 for (; ii < p999; ii++)
            //                     sum += latency_manager[ii];
            //                 l999 = sum / p999;
            //                 Log_info("50.0%% LATENCY: %lf; 90.0%% LATENCY: %lf; 99.0%% LATENCY: %lf; 99.9%% LATENCY: %lf\n", l50 * 1000, l90 * 1000, l99 * 1000, l999 * 1000);

            //                 break;
            //             }

            //             /*
            //                 chaining
            //             */
            //             while (true){
            //                 if (cnt-num_done >= max_pending_requests || last_sent >= batchSize){
            //                     // Log_info("pausing the one-time sending job; cnt: %d, num_done:%d", cnt, num_done);
            //                     Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 2));
            //                     // Log_info("woke up from wait");
            //                     last_sent = 0;
            //                     break;
            //                 }
            //                 cnt = crpc_nop_counter.next();
            //                 last_sent++;
            //                 // int q = proxies.size()/2+1;
            //                 if (cnt % 200000 == 0) {
            //                     Log_info("%d crpc nop requests", cnt);
            //                 }
            //                 struct timespec t_buf;
            //                 clock_gettime(CLOCK_REALTIME, &t_buf);
            //                 response_saver[cnt] = t_buf;
            //                 // state vector
            //                 std::vector<ResultAdd> state;


            //                 auto f = proxies[sitesInfo_[0]]->async_CrpcLargePayload(cnt, payload, sitesInfo_, state);
            //                 Future::safe_release(f);
            //             }

            //         }
            //     })));

            //     prevPoll_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>([this, max_pending_requests, seconds](){
            //         Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 6));
            //         auto beg_time = Time::now();
            //         auto end_time1 = beg_time + (seconds/3) * pow(10, 6);
            //         auto end_time2 = beg_time + (seconds*2/3) * pow(10,6);
            //         struct timespec start_;
            //         start_.tv_sec = 0;
            //         int last_sent = 0;

            //         while(true) {
            //             auto cur_time = Time::now(); // optimize: this call is not scalable.
                        
            //             if (cur_time > end_time1) {
            //                 cnt33 = crpc_num_done.peek_next();
            //                 Log_info("one third time done, cnt33: %d", cnt33);
            //                 break;
            //             }
            //             else {
            //                 Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
            //             }
            //         }

            //         while(true) {
            //             auto cur_time = Time::now(); // optimize: this call is not scalable.
                        
            //             if (cur_time > end_time2) {
            //                 cnt66 = crpc_num_done.peek_next();
            //                 Log_info("two third time done, cnt66: %d", cnt66);
            //                 break;
            //             }
            //             else {
            //                 Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
            //             }
            //         }
            //     })));
            //     // }
            // }
            // else { // broadcast
            //     // for (uint32_t n_tx = 0; n_tx < 10; n_tx++) {
            //     prevPoll_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>([this, batchSize, max_pending_requests, seconds, payload](){
            //     Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 7)*2);
            //     Log_info("in bcast; tid: %d", gettid());
            //     auto beg_time = Time::now() ;
            //     auto end_time = beg_time + seconds * pow(10, 6);
            //     struct timespec start_;
            //     start_.tv_sec = 0;
            //     int last_sent = 0;
            //     while(true){
            //         // Log_info("Inside the loop");
            //         auto cur_time = Time::now(); // optimize: this call is not scalable.
            //         int cnt = nop_counter.peek_next();
            //         int num_done = bcast_num_done.peek_next();
            //         if (cur_time > end_time) {
            //             Log_info("***** ENDING Bcast*****");
            //             while (cnt !=num_done){
            //                 Log_info("#### Waiting for all the responses to return; cnt: %d, num_done: %d", cnt, num_done);
            //                 Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
            //                 num_done = bcast_num_done.peek_next();                            
            //             }
            //             Log_info("throughput: %f", (double)(cnt66 - cnt33) / (seconds * 2.0 / 3 - seconds / 3.0));

            //             ///////////////////////////
            //             // latency calculation            

            //             std::sort(latency_manager.begin(), latency_manager.end());
            //             size_t l_length = latency_manager.size();
            //             double sum = 0.0;
            //             int p50 = floor(l_length * 0.5);
            //             int p90 = floor(l_length * 0.90);
            //             int p99 = floor(l_length * 0.99);
            //             int p999 = floor(l_length * 0.999);
            //             double l50, l90, l99, l999;
            //             int ii = 0;
            //             for (; ii < p50; ii++)
            //                 sum += latency_manager[ii];
            //             l50 = sum / p50;
            //             for (; ii < p90; ii++)
            //                 sum += latency_manager[ii];
            //             l90 = sum / p90;
            //             for (; ii < p99; ii++)
            //                 sum += latency_manager[ii];
            //             l99 = sum / p99;
            //             for (; ii < p999; ii++)
            //                 sum += latency_manager[ii];
            //             l999 = sum / p999;
            //             Log_info("50.0%% LATENCY: %lf; 90.0%% LATENCY: %lf; 99.0%% LATENCY: %lf; 99.9%% LATENCY: %lf\n", l50 * 1000, l90 * 1000, l99 * 1000, l999 * 1000);


            //             ///////////////////////////
            //             break;
            //         }

            //         while (true){
            //             if (cnt-num_done >= max_pending_requests || last_sent >= batchSize){
            //                 // Log_info("pausing the one-time sending job; cnt: %d, num_done:%d", cnt, num_done);
            //                 Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 2));
            //                 // Log_info("woke up from wait");
            //                 last_sent = 0;
            //                 break;
            //             }
            //             last_sent++;
            //             cnt = nop_counter.next();
            //             // int q = proxies.size()/2+1;
            //             if (cnt % 200000 == 0) {
            //                 Log_info("%d bcast nop requests", cnt);
            //             }
            //             int q = proxies.size()/2; // change add +1 back
            //             auto numMutex2 = std::make_shared<std::mutex>();
            //             struct timespec t_buf;
            //             clock_gettime(CLOCK_REALTIME, &t_buf);
            //             response_saver[cnt] = t_buf;
            //             for (int i=1; i<proxies.size(); i++){
            //                 FutureAttr fu_attr;
            //                 // Log_info("#### inside loop, value of num: %d, and cnt: %d", num, cnt);
            //                 fu_attr.callback = [numMutex2, q, this, cnt] (Future* fu) {
            //                     std::lock_guard<std::mutex> lock(*numMutex2);
            //                     // Log_info("#### inside callback, value of cnt: %d", cnt);
            //                     if (fu->get_error_code() != 0) {
            //                         Log_info("#### some ERROR, sending back response");
            //                         // defer->reply();
            //                         return;
            //                     }
            //                     this->response_counter[cnt] = this->response_counter[cnt]+1;
            //                     // Log_info("#### inside callback, value of response_counter: %ld", this->response_counter[cnt]);
            //                     if (this->response_counter[cnt] == q){
            //                         auto it = response_saver.find(cnt);

            //                         struct timespec t_buf;
            //                         clock_gettime(CLOCK_REALTIME, &t_buf);
            //                         auto last_time = it->second;
            //                         latency_manager.push_back(t_buf.tv_sec - last_time.tv_sec + (t_buf.tv_nsec - last_time.tv_nsec) / 1000000000.0);
            //                         response_saver.erase(it);
            //                         // Log_info("#### sending reply");
            //                         // Log_info("#### inside callback, value of response_counter: %ld", this->response_counter[cnt]);
            //                         // defer->reply();
            //                         int num = bcast_num_done.next();
            //                         // Log_info("#### sending reply; id done: %d", num);
            //                     }
            //                     // Log_info("ending callback, value of cnt: %d", cnt);
            //                 };
            //                 auto f = proxies[i]->async_BroadcastLargePayload(cnt, payload, fu_attr);
            //                 Future::safe_release(f);
                            
            //             }

            //         }
            //     }
            //     })));

            //     prevPoll_->add(dynamic_pointer_cast<Job>(std::make_shared<OneTimeJob>([this, max_pending_requests, seconds](){
            //         Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 6));
            //         auto beg_time = Time::now();
            //         auto end_time1 = beg_time + (seconds/3) * pow(10, 6);
            //         auto end_time2 = beg_time + (seconds*2/3) * pow(10,6);
            //         struct timespec start_;
            //         start_.tv_sec = 0;
            //         int last_sent = 0;

            //         while(true) {
            //             auto cur_time = Time::now(); // optimize: this call is not scalable.
                        
            //             if (cur_time > end_time1) {
            //                 cnt33 = bcast_num_done.peek_next();
            //                 Log_info("one third time done, cnt33: %d", cnt33);
            //                 break;
            //             }
            //             else {
            //                 Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
            //             }
            //         }

            //         while(true) {
            //             auto cur_time = Time::now(); // optimize: this call is not scalable.
                        
            //             if (cur_time > end_time2) {
            //                 cnt66 = bcast_num_done.peek_next();
            //                 Log_info("two third time done, cnt66: %d", cnt66);
            //                 break;
            //             }
            //             else {
            //                 Reactor::CreateSpEvent<NeverEvent>()->Wait(pow(10, 4));
            //             }
            //         }
            //     })));
            //     // }
            // }

        }

        void CrpcAdd(const uint64_t& id, const int64_t& value1, const int64_t& value2, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, rrr::DeferredReply* defer) override {
            // static bool hasPrinted = false; // Static variable to track if it has printed

            // if (!hasPrinted)
            // {
            //     Log_info("in CrpcAdd; tid: %d", gettid());
            //     hasPrinted = true; // Update the static variable
            // }
            // if (self_addr_ == svr_addrs_[0]){
            // Log_info("!!!! server: %s, CrpcAdd number: %ld, tid: %d", self_addr_, id, gettid());
            // }
            // int q = proxies.size()/2+1;
            // if (addrChain.size() == 1) {
            //     // Log_info("==== reached the final link in the chain");
            //     if (crpc_response_counter.find(id) == crpc_response_counter.end()){
            //         return;
            //     }
                
            //     auto defer = crpc_response_counter[id];
            //     crpc_response_counter.erase(id);

            //     if (state.size() >= q && defer){
            //         // Log_info("responding to the client finally");
            //         defer->reply();
            //     }
            //     return;
            // }
        
            // ResultAdd res;
            // res.result = value1 + value2;
            // std::vector<ResultAdd> st(state);
            // st.push_back(res);

            // std::vector<uint16_t> addrChainCopy(addrChain.begin() + 1, addrChain.end());

            // if (st.size() == q){
            //     // Log_info("Current value of k: %d", k);
            //     auto temp_addrChain = std::vector<uint16_t>{addrChainCopy.back()};
            //     proxies[addrChainCopy.back()]->async_CrpcAdd(id, value1, value2, temp_addrChain, st);
            // }

            // proxies[addrChainCopy[0]]->async_CrpcAdd(id, value1, value2, addrChainCopy, st);

            // // new code

            int q = proxies.size() / 2; // change later
            if (addrChain.size() == 1) {
                auto it = response_saver.find(id);
                if (it == response_saver.end()){
                    return;
                }

                struct timespec t_buf;
                clock_gettime(CLOCK_REALTIME, &t_buf);
                auto last_time = it->second;
                latency_manager.push_back(t_buf.tv_sec - last_time.tv_sec + (t_buf.tv_nsec - last_time.tv_nsec) / 1000000000.0);
                response_saver.erase(it);
                // auto defer1 = it->second;
                // crpc_response_counter.erase(it);

                if (state.size() >= q){
                    // Log_info("!!!! Sending back the reply");
                    // defer1->reply();
                    crpc_num_done.next();
                }
                return;
            }

            ResultAdd res{value1 + value2};
            std::vector<ResultAdd> st;
            st.reserve(state.size() + 1);  // Exactly reserve space for all elements of 'state' plus one more
            st.insert(st.end(), state.begin(), state.end());
            st.emplace_back(std::move(res));

            std::vector<uint16_t> addrChainCopy;
            addrChainCopy.reserve(addrChain.size() - 1);  // Reserve space to avoid reallocation
            std::copy(addrChain.begin() + 1, addrChain.end(), std::back_inserter(addrChainCopy));

            if (st.size() == q){
                proxies[addrChainCopy.back()]->async_CrpcAdd(id, value1, value2, {addrChainCopy.back()}, st);
            }

            if (addrChainCopy.size() == 1){ // don't send the data back to the leader
                defer->reply();
                return;
            }

            proxies[addrChainCopy[0]]->async_CrpcAdd(id, value1, value2, addrChainCopy, st);
            defer->reply();
        }

        void BroadcastAdd(const int64_t& n, const int64_t& value1, const int64_t& value2, int64_t* result, rrr::DeferredReply* defer) override {
            // Log_info("server: %s, BroadcastAdd number: %ld", self_addr_, n);
            // static bool hasPrinted = false; // Static variable to track if it has printed

            // if (!hasPrinted)
            // {
            //     Log_info("in BroadcastAdd; tid: %d", gettid());
            //     hasPrinted = true; // Update the static variable
            // }
            *result = value1 + value2;
            defer->reply();
        }

        void CrpcLargePayload(const uint64_t& id, const std::string& str, const std::vector<uint16_t>& addrChain, const std::vector<ResultAdd>& state, rrr::DeferredReply* defer) override {
            // Log_info("server: %s, CrpcAdd number: %ld", self_addr_, id);
            int q = proxies.size() / 2; // change this later to + 1
            if (addrChain.size() == 1) {
                auto it = response_saver.find(id);
                if (it == response_saver.end()){
                    return;
                }

                struct timespec t_buf;
                clock_gettime(CLOCK_REALTIME, &t_buf);
                auto last_time = it->second;
                latency_manager.push_back(t_buf.tv_sec - last_time.tv_sec + (t_buf.tv_nsec - last_time.tv_nsec) / 1000000000.0);
                response_saver.erase(it);

                if (state.size() >= q){
                    crpc_num_done.next();
                }
                return;
            }
            // int q = proxies.size()/2+1;
            // if (addrChain.size() == 1) {
            //     // Log_info("==== reached the final link in the chain");
            //     if (crpc_response_counter.find(id) == crpc_response_counter.end()){
            //         return;
            //     }
                
            //     auto defer = crpc_response_counter[id];
            //     crpc_response_counter.erase(id);

            //     if (state.size() >= q && defer){
            //         // Log_info("responding to the client finally");
            //         defer->reply();
            //     }
            //     return;
            // }

            ResultAdd res{10};
            std::vector<ResultAdd> st;
            st.reserve(state.size() + 1);  // Exactly reserve space for all elements of 'state' plus one more
            st.insert(st.end(), state.begin(), state.end());
            st.emplace_back(std::move(res));

            std::vector<uint16_t> addrChainCopy;
            addrChainCopy.reserve(addrChain.size() - 1);  // Reserve space to avoid reallocation
            std::copy(addrChain.begin() + 1, addrChain.end(), std::back_inserter(addrChainCopy));

            if (st.size() == q){
                string emptyStr = "";
                proxies[addrChainCopy.back()]->async_CrpcLargePayload(id,  emptyStr, {addrChainCopy.back()}, st);
            }

            proxies[addrChainCopy[0]]->async_CrpcLargePayload(id, str, addrChainCopy, st);        

            defer->reply();
            // ResultAdd res;
            // res.result = 10;
            // // auto r = Coroutine::CreateRun([&]() { this->OnAdd(value1, value2, &res.result, []() {}); }); // #profile - 2.88%
            // // this->OnAdd(value1, value2, &res.result, []() {}); // #profile - 2.88%
            // std::vector<ResultAdd> st(state);
            // st.push_back(res);

            // std::vector<uint16_t> addrChainCopy(addrChain.begin() + 1, addrChain.end());

            // if (st.size() == q){
            //     // Log_info("Current value of k: %d", k);
            //     auto temp_addrChain = std::vector<uint16_t>{addrChainCopy.back()};
            //     proxies[addrChainCopy.back()]->async_CrpcLargePayload(id, str, temp_addrChain, st);
            // }

            // proxies[addrChainCopy[0]]->async_CrpcLargePayload(id, str, addrChainCopy, st);
            // Log_info("*** inside SampleCrpcServer::OnCRPC; cp 3 tid: %d", gettid());
        }

        void BroadcastLargePayload(const int64_t& n, const std::string&, int64_t* result, rrr::DeferredReply* defer) override {
            // Log_info("server: %s, BroadcastAdd number: %ld", self_addr_, n);
            *result = 10;
            defer->reply();
        }

        void nop(const std::string&, rrr::DeferredReply* defer) override {
            static bool hasPrinted = false; // Static variable to track if it has printed

            if (!hasPrinted)
            {
                Log_info("in nop; tid: %d", gettid());
                hasPrinted = true; // Update the static variable
            }
            // Log_info("sending request to all the servers, bcast");
            int q = proxies.size()/2; // change add +1 back
            int cnt = nop_counter.next();
            if (cnt % 200000 == 0) {
                Log_info("%d nop requests", cnt);
            }
            auto numMutex2 = std::make_shared<std::mutex>();
            // for (auto proxy: proxies){
            for (int i=0; i<proxies.size(); i++){
                FutureAttr fu_attr;
                // Log_info("#### inside loop, value of num: %d, and cnt: %d", num, cnt);
                fu_attr.callback = [numMutex2, q, defer, this, cnt] (Future* fu) {
                    std::lock_guard<std::mutex> lock(*numMutex2);
                    // Log_info("#### inside callback, value of cnt: %d", cnt);
                    if (fu->get_error_code() != 0) {
                        Log_info("#### some ERROR, sending back response");
                        defer->reply();
                        return;
                    }
                    this->response_counter[cnt] = this->response_counter[cnt]+1;
                    // Log_info("#### inside callback, value of response_counter: %ld", this->response_counter[cnt]);
                    if (this->response_counter[cnt] == q && defer){
                        // Log_info("#### sending reply");
                        // Log_info("#### inside callback, value of response_counter: %ld", this->response_counter[cnt]);
                        defer->reply();
                    }
                    // Log_info("ending callback, value of cnt: %d", cnt);
                };
                auto f = proxies[i]->async_BroadcastAdd(cnt, 2, 3, fu_attr);
                Future::safe_release(f);
            }

            // Log_info("done broadcasting");
        }

        void crpc_nop(const std::string&, rrr::DeferredReply* defer) override {
            // Log_info("!!!! sending request to all the servers");
            static bool hasPrinted = false; // Static variable to track if it has printed

            if (!hasPrinted)
            {
                Log_info("in crpc_nop; tid: %d", gettid());
                hasPrinted = true; // Update the static variable
            }
            // int q = proxies.size()/2+1;
            int cnt = crpc_nop_counter.next();
            if (cnt % 200000 == 0) {
                Log_info("%d crpc nop requests", cnt);
            }
            // chain address vector
            // // moved to the setup above

            // Log_info("size of sites_info_: %d and proxies_size: %d", sitesInfo_.size(), proxies.size());

            // state vector
            std::vector<ResultAdd> state;

            // for (auto proxy: proxies){
                
            // }


            auto f = proxies[sitesInfo_[0]]->async_CrpcAdd(cnt, 2, 3, sitesInfo_, state);
            Future::safe_release(f);

            crpc_response_counter[cnt] = defer;

            // Log_info("done broadcasting");
        }


        void nop_largePayload(const std::string& str, rrr::DeferredReply* defer) override {
            // Log_info("sending request to all the servers");
            int q = proxies.size()/2+1;
            int cnt = nop_counter.next();
            if (cnt % 200 == 0) {
                Log_info("%d nop requests", cnt);
            }
            auto numMutex2 = std::make_shared<std::mutex>();
            std::string large_payload(one_megabyte, character_to_repeat); 
            for (auto proxy: proxies){
                FutureAttr fu_attr;
                // Log_info("#### inside loop, value of num: %d, and cnt: %d", num, cnt);
                fu_attr.callback = [numMutex2, q, defer, this, cnt] (Future* fu) {
                    std::lock_guard<std::mutex> lock(*numMutex2);
                    // Log_info("#### inside callback, value of cnt: %d", cnt);
                    if (fu->get_error_code() != 0) {
                        Log_info("#### some ERROR, sending back response");
                        defer->reply();
                        return;
                    }
                    this->response_counter[cnt] = this->response_counter[cnt]+1;
                    // Log_info("#### inside callback, value of response_counter: %ld", this->response_counter[cnt]);
                    if (this->response_counter[cnt] == q && defer){
                        // Log_info("#### sending reply");
                        // Log_info("#### inside callback, value of response_counter: %ld", this->response_counter[cnt]);
                        defer->reply();
                    }
                    // Log_info("ending callback, value of cnt: %d", cnt);
                };
                auto f = proxy.second->async_BroadcastLargePayload(cnt, large_payload, fu_attr);
                Future::safe_release(f);
            }

            // Log_info("done broadcasting");
        }

        void crpc_nop_largePayload(const std::string& str, rrr::DeferredReply* defer) override {
            // Log_info("sending request to all the servers");

            int q = proxies.size()/2+1;
            int cnt = crpc_nop_counter.next();
            if (cnt % 200 == 0) {
                Log_info("%d crpc nop requests", cnt);
            }
            // chain address vector
            std::vector<uint16_t> sitesInfo_(proxies.size());
            std::iota(sitesInfo_.begin(), sitesInfo_.end(), 0);
            sitesInfo_.push_back(0);

            // Log_info("size of sites_info_: %d and proxies_size: %d", sitesInfo_.size(), proxies.size());

            // state vector
            std::vector<ResultAdd> state;

            std::string large_payload(one_megabyte, character_to_repeat); 

            auto f = proxies[sitesInfo_[0]]->async_CrpcLargePayload(cnt, large_payload, sitesInfo_, state);
            Future::safe_release(f);

            crpc_response_counter[cnt] = defer;

            // Log_info("done broadcasting");
        }
};
}