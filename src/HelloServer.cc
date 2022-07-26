
#include "__dep__.h"
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <thread>
#include <string>
#include <cstring>
#include <unistd.h>
#include "example/example_impl.h"
#include <pthread.h>
#include<rpc.h>

using namespace example_client;
using namespace janus;

erpc::Rpc<erpc::CTransport> *rpc;
erpc::MsgBuffer req;
erpc::MsgBuffer resp;

int run_server() {
    ExampleClientServiceImpl *impl = new ExampleClientServiceImpl();
    rrr::PollMgr *pm = new rrr::PollMgr();
    base::ThreadPool *tp = new base::ThreadPool();
    rrr::Server *server = new rrr::Server(pm, tp);
    server->reg(impl);
    string s1 = "127.0.0.1:8090";
    server->start(s1.c_str());
    while (1) {
        sleep(1);
    }
}

int main(){
    run_server();
    return 0;
}
