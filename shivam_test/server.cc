#include "demo_impl.h"

int main(){
	demo::DemoServiceImpl impl;

	rrr::PollMgr *pm = new rrr::PollMgr();
	base::ThreadPool *tp = new base::ThreadPool();
	rrr::Server *server = new rrr::Server(pm, tp);
	server->reg(&impl);
	server->start(std::string("127.0.0.1:8090").c_str());

	while(1){
		sleep(1);
	}
}


