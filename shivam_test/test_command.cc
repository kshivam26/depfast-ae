#include "test_command.h"

namespace janus {

static int volatile x1 =
    MarshallDeputy::RegInitializer(MarshallDeputy::CMD_TEST,
                                     [] () -> Marshallable* {
                                       return new TestCommand;
                                     });

    Marshal& TestCommand::ToMarshal(Marshal& m) const{
        Log_info("inside TestCommand::ToMarshal; checkpoint 0");
        m << hi;
        m << *reply;
        Log_info("inside TestCommand::ToMarshal; checkpoint 1");
        // MarshallDeputy md(cmd_);
        Log_info("inside TestCommand::ToMarshal; checkpoint 2");
        // m << md;
        Log_info("inside TestCommand::ToMarshal; checkpoint 3");
        return m;
    }

    Marshal& TestCommand::FromMarshal(Marshal& m){
        Log_info("inside TestCommand::FromMarshal; checkpoint 3");
        m >> hi;
        m >> *reply;
        // MarshallDeputy md;
        // m >> md;
        // if (!cmd_)
        //     cmd_=md.sp_data_;
        Log_info("inside TestCommand::FromMarshal; value of hi= %s", hi);
        return m;
    }
}