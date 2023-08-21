#include "append_entries_command.h"

namespace janus {

// class SimpleCommand;

static int volatile x1 =
    MarshallDeputy::RegInitializer(MarshallDeputy::CMD_SAMPLE_CRPC_APPEND_ENTRIES,
                                     [] () -> Marshallable* {
                                       return new AppendEntriesCommandToyCrpc;
                                     });

static int volatile x2 =
    MarshallDeputy::RegInitializer(MarshallDeputy::CMD_SAMPLE_CRPC_APPEND_ENTRIES_STATE,
                                     [] () -> Marshallable* {
                                       return new AppendEntriesCommandStateToyCrpc;
                                     });

    Marshal& AppendEntriesCommandToyCrpc::ToMarshal(Marshal& m) const{
        // Log_debug("==== inside AppendEntriesCommandToyCrpc::ToMarshal; checkpoint 0 @ %d", gettid());
        m << md_cmd; // #profile - 0.66%
        // Log_info("==== inside AppendEntriesCommandToyCrpc::ToMarshal; checkpoint 3");
        return m;
    }

    Marshal& AppendEntriesCommandToyCrpc::FromMarshal(Marshal& m){
        // Log_debug("==== inside AppendEntriesCommandToyCrpc::FromMarshal; checkpoint 1 @ %d", gettid());
        // Log_info("==== inside AppendEntriesCommandToyCrpc::FromMarshal; checkpoint 3");
        m >> md_cmd; // #profile - 2.05%

        // Log_info("==== inside AppendEntriesCommandToyCrpc::FromMarshal; checkpoint 4");
        return m;
    }


    Marshal& AppendEntriesCommandStateToyCrpc::ToMarshal(Marshal& m) const{
        // Log_debug("==== inside AppendEntriesCommandStateToyCrpc::ToMarshal; checkpoint 0 @ %d", gettid());
        m << results;
        // Log_info("==== inside AppendEntriesCommandStateToyCrpc::ToMarshal; checkpoint 3");
        return m;
    }

    Marshal& AppendEntriesCommandStateToyCrpc::FromMarshal(Marshal& m){
        // Log_debug("==== inside AppendEntriesCommandStateToyCrpc::FromMarshal; checkpoint 1 @ %d", gettid());
        m >> results;
        // Log_info("==== inside AppendEntriesCommandStateToyCrpc::FromMarshal; checkpoint 2");
        return m;
    }
}