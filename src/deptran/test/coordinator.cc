#include "../__dep__.h"
#include "../constants.h"
#include "coordinator.h"
#include "commo.h"
#include "server.h"

namespace janus {

CoordinatorTest::CoordinatorTest(uint32_t coo_id,
                                 int32_t benchmark,
                                 ClientControlServiceImpl *ccsi,
                                 uint32_t thread_id) : Coordinator(coo_id, benchmark, ccsi, thread_id) {

  Log_info("*** inside CoordinatorTest");
}

CoordinatorTest::~CoordinatorTest() {
  Log_info("Test coord %d destroyed", (int)coo_id_);
}

}
