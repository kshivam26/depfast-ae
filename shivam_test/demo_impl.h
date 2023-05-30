#include "demo.h"

namespace demo{

class DemoServiceImpl: public DemoService {
	public:
	
	void hello(const std::string& hi, std::string* reply, rrr::DeferredReply* defer);
	void sum(const rrr::i32& a, const rrr::i32& b, const rrr::i32& c, rrr::i32* result, rrr::DeferredReply* defer);
};
}
		
			
