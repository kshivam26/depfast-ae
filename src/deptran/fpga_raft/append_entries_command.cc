#include "append_entries_command.h"

namespace janus {

// class SimpleCommand;

static int volatile x1 =
    MarshallDeputy::RegInitializer(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES,
                                     [] () -> Marshallable* {
                                       return new AppendEntriesCommand;
                                     });

static int volatile x2 =
    MarshallDeputy::RegInitializer(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES_STATE,
                                     [] () -> Marshallable* {
                                       return new AppendEntriesCommandState;
                                     });

    Marshal& AppendEntriesCommand::ToMarshal(Marshal& m) const{
        // Log_debug("==== inside AppendEntriesCommand::ToMarshal; checkpoint 0 @ %d", gettid());
        m << slot;
        m << ballot; 
        m << leaderCurrentTerm;
        m << leaderPrevLogIndex;
        m << leaderPrevLogTerm;
        m << leaderCommitIndex;
        m << dep_id;
        m << md_cmd; // #profile - 0.66%
        // Log_info("==== inside AppendEntriesCommand::ToMarshal; checkpoint 3");
        return m;
    }

    Marshal& AppendEntriesCommand::FromMarshal(Marshal& m){
        // Log_debug("==== inside AppendEntriesCommand::FromMarshal; checkpoint 1 @ %d", gettid());
        m >> slot;
        m >> ballot; 
        m >> leaderCurrentTerm;
        m >> leaderPrevLogIndex;
        m >> leaderPrevLogTerm;
        m >> leaderCommitIndex;
        m >> dep_id;
        // Log_info("==== inside AppendEntriesCommand::FromMarshal; checkpoint 3");
        m >> md_cmd; // #profile - 2.05%

        // Log_info("==== inside AppendEntriesCommand::FromMarshal; checkpoint 4");
        return m;
    }


    Marshal& AppendEntriesCommandState::ToMarshal(Marshal& m) const{
        // Log_debug("==== inside AppendEntriesCommandState::ToMarshal; checkpoint 0 @ %d", gettid());
        m << results;
        // Log_info("==== inside AppendEntriesCommandState::ToMarshal; checkpoint 3");
        return m;
    }

    Marshal& AppendEntriesCommandState::FromMarshal(Marshal& m){
        // Log_debug("==== inside AppendEntriesCommandState::FromMarshal; checkpoint 1 @ %d", gettid());
        m >> results;
        // Log_info("==== inside AppendEntriesCommandState::FromMarshal; checkpoint 2");
        return m;
    }
}



// #include "FpgaRaft_AppendEntries_command.h"


// namespace janus {


// static int volatile x1 =
// MarshallDeputy::RegInitializer(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES,
// [] () -> Marshallable* {return new AppendEntriesCommand;});

// static int volatile x2 =
// MarshallDeputy::RegInitializer(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES_STATE,
// [] () -> Marshallable* {return new AppendEntriesCommandState;});

// Marshal& AppendEntriesCommand::ToMarshal(Marshal& m) const{
//     m << slot;
//     m << ballot;
//     m << leaderCurrentTerm;
//     m << leaderPrevLogIndex;
//     m << leaderPrevLogTerm;
//     m << leaderCommitIndex;
//     m << dep_id;
//     m << md_cmd;
//     return m;
// }

// Marshal& AppendEntriesCommand::FromMarshal(Marshal& m) {
//     m >> slot;
//     m >> ballot;
//     m >> leaderCurrentTerm;
//     m >> leaderPrevLogIndex;
//     m >> leaderPrevLogTerm;
//     m >> leaderCommitIndex;
//     m >> dep_id;
//     m >> md_cmd;
//     return m;
// }
// Marshal& AppendEntriesCommandState::ToMarshal(Marshal& m) const{
//     m << results;
//     return m;
// }

// Marshal& AppendEntriesCommandState::FromMarshal(Marshal& m) {
//     m >> results;
//     return m;
// }

// } // namespace janus

