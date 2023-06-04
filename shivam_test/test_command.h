#include "../src/deptran/marshallable.h"

namespace janus{
class TestCommand : public Marshallable {
 public:
  TestCommand() : Marshallable(MarshallDeputy::CMD_TEST) {
  }
  // shared_ptr<Marshallable> cmd_{nullptr};
  std::string hi;
  std::string* reply;
  Marshal& ToMarshal(Marshal&) const override;
  Marshal& FromMarshal(Marshal&) override;
};

class SumCommand : public Marshallable {
 public:
  SumCommand() : Marshallable(MarshallDeputy::CMD_TEST) {
  }
  // shared_ptr<Marshallable> cmd_{nullptr};
  std::string hi;
  Marshal& ToMarshal(Marshal&) const override;
  Marshal& FromMarshal(Marshal&) override;
};

}