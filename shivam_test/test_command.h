#include "../src/deptran/marshallable.h"

namespace janus{
class TestCommand : public Marshallable {
 public:
  TestCommand() : Marshallable(MarshallDeputy::CMD_TEST) {
  }
  // shared_ptr<Marshallable> cmd_{nullptr};
  rrr::i32 a;
  rrr::i32 b;
  rrr::i32 c;
  // rrr::i32 result;
  Marshal& ToMarshal(Marshal&) const override;
  Marshal& FromMarshal(Marshal&) override;
};

class TestCommandState : public Marshallable {
 public:
  TestCommandState() : Marshallable(MarshallDeputy::CMD_TEST_STATE) {
  }
  // shared_ptr<Marshallable> cmd_{nullptr};
  std::vector<rrr::i32> results;
  Marshal& ToMarshal(Marshal&) const override;
  Marshal& FromMarshal(Marshal&) override;
};

}