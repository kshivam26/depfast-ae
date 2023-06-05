#include "test_command.h"

namespace janus {

static int volatile x1 =
    MarshallDeputy::RegInitializer(MarshallDeputy::CMD_TEST,
                                     [] () -> Marshallable* {
                                       return new TestCommand;
                                     });

static int volatile x2 =
    MarshallDeputy::RegInitializer(MarshallDeputy::CMD_TEST_STATE,
                                     [] () -> Marshallable* {
                                       return new TestCommandState;
                                     });

    Marshal& TestCommand::ToMarshal(Marshal& m) const{
        Log_info("inside TestCommand::ToMarshal; checkpoint 0");
        m << a;
        m << b; 
        m << c;
        //m << *result;
        Log_info("inside TestCommand::ToMarshal; checkpoint 1");
        // MarshallDeputy md(cmd_);
        Log_info("inside TestCommand::ToMarshal; checkpoint 2");
        // m << md;
        Log_info("inside TestCommand::ToMarshal; checkpoint 3");
        return m;
    }

    Marshal& TestCommand::FromMarshal(Marshal& m){
        Log_info("inside TestCommand::FromMarshal; checkpoint 1");
        m >> a;
        Log_info("inside TestCommand::FromMarshal; checkpoint 2");
        m >> b; 
        Log_info("inside TestCommand::FromMarshal; checkpoint 3");
        m >> c;
        //m >> *result;
        // MarshallDeputy md;
        // m >> md;
        // if (!cmd_)
        //     cmd_=md.sp_data_;
        Log_info("inside TestCommand::FromMarshal; checkpoint 4");
        return m;
    }


    Marshal& TestCommandState::ToMarshal(Marshal& m) const{
        Log_info("inside TestCommandState::ToMarshal; checkpoint 0");
        m << results;
        Log_info("inside TestCommandState::ToMarshal; checkpoint 3");
        return m;
    }

    Marshal& TestCommandState::FromMarshal(Marshal& m){
        Log_info("inside TestCommandState::FromMarshal; checkpoint 1");
        m >> results;
        Log_info("inside TestCommandState::FromMarshal; checkpoint 2");
        return m;
    }
}