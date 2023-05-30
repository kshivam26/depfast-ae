#include "demo_impl.h"

namespace demo{

	void DemoServiceImpl::hello(const std::string& hi, std::string* reply, rrr::DeferredReply* defer){
		printf("inside demoserviceimpl::hello");
		*reply += std::string("Re123: ") + hi;
		defer->reply();
		printf("returning demoserviceimpl::hello");
	}

	void DemoServiceImpl::sum(const rrr::i32& a, const rrr::i32& b, const rrr::i32& c, rrr::i32* result, rrr::DeferredReply* defer){
		*result = a + b + c;
		defer->reply();
	}
}
