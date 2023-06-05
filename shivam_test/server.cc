#include "demo_impl.h"


int main(){
	// demo::DemoServiceImpl impl;

	std::vector<std::string> serverAddr;
	serverAddr.push_back("127.0.0.1:8091");
	serverAddr.push_back("127.0.0.1:8092");
	serverAddr.push_back("127.0.0.1:8093");
	serverAddr.push_back("127.0.0.1:8094");

	

	rrr::PollMgr *pm1 = new rrr::PollMgr();
	base::ThreadPool *tp1 = new base::ThreadPool(4);
	rrr::Server *server1 = new rrr::Server(pm1, tp1);
	server1->reg(new janus::DemoServiceImpl(server1, serverAddr));
	server1->start(serverAddr[0].c_str());


	// rrr::PollMgr *pm = new rrr::PollMgr();

	// std::shared_ptr<rrr::Client> client = std::make_shared<rrr::Client>(pm);
	// auto connect_result = client->connect(serverAddr[0].c_str(), false);
	// if (connect_result != 0)
	// {
	// 	printf("connection unsuccessful\n");
	// }
	// else
	// {
	// 	printf("connection successful\n");
	// }
	// printf("checkpoint-0\n");
	// auto proxy = new demo::DemoProxy(client.get());


	rrr::PollMgr *pm2 = new rrr::PollMgr();
	base::ThreadPool *tp2 = new base::ThreadPool(4);
	rrr::Server *server2 = new rrr::Server(pm2, tp2);
	server2->reg(new janus::DemoServiceImpl(server2, serverAddr));
	server2->start(serverAddr[1].c_str());

	rrr::PollMgr *pm3 = new rrr::PollMgr();
	base::ThreadPool *tp3 = new base::ThreadPool(4);
	rrr::Server *server3 = new rrr::Server(pm3, tp3);
	server3->reg(new janus::DemoServiceImpl(server3, serverAddr));
	server3->start(serverAddr[2].c_str());

	rrr::PollMgr *pm4 = new rrr::PollMgr();
	base::ThreadPool *tp4 = new base::ThreadPool(4);
	rrr::Server *server4 = new rrr::Server(pm4, tp4);
	server4->reg(new janus::DemoServiceImpl(server4, serverAddr));
	server4->start(serverAddr[3].c_str());

	while(1){
		sleep(1);
	}
}


