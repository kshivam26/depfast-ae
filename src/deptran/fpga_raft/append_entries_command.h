#pragma once
#include "../marshallable.h"
#include "../procedure.h"
#include "../rcc/tx.h"
#include "../rcc_rpc.h"


namespace janus{

class AppendEntriesCommand : public Marshallable {
    public:
    AppendEntriesCommand() : Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES) {
    }
  
    AppendEntriesCommand(uint64_t slot, 
                        ballot_t ballot,
                        uint64_t leaderCurrentTerm,
                        uint64_t leaderPrevLogIndex,
                        uint64_t leaderPrevLogTerm ,
                        uint64_t leaderCommitIndex,
                        DepId dep_id,
                        MarshallDeputy md_cmd) : 
        Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES),
        slot(slot),
        ballot(ballot),
        leaderCurrentTerm(leaderCurrentTerm),
        leaderPrevLogIndex(leaderPrevLogIndex),
        leaderPrevLogTerm(leaderPrevLogTerm),
        leaderCommitIndex(leaderCommitIndex),
        dep_id(dep_id),
        md_cmd(md_cmd) {}
    uint64_t slot;
    ballot_t ballot;
    uint64_t leaderCurrentTerm;
    uint64_t leaderPrevLogIndex;
    uint64_t leaderPrevLogTerm;
    uint64_t leaderCommitIndex;
    DepId dep_id;
    MarshallDeputy md_cmd;

    Marshal& ToMarshal(Marshal&) const override;
    Marshal& FromMarshal(Marshal&) override;
};

class AppendEntriesCommandState : public Marshallable {
    public:
    AppendEntriesCommandState() : Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES_STATE) {
    }

    std::vector<AppendEntriesResult> results = {};
    Marshal& ToMarshal(Marshal&) const override;
    Marshal& FromMarshal(Marshal&) override;
};

}


// // generated file below

// namespace janus {

// class AppendEntriesCommand: public Marshallable {
//     public:
//     AppendEntriesCommand() : Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES) {}

//     AppendEntriesCommand(
//     uint64_t slot,
//     ballot_t ballot,
//     uint64_t leaderCurrentTerm,
//     uint64_t leaderPrevLogIndex,
//     uint64_t leaderPrevLogTerm,
//     uint64_t leaderCommitIndex,
//     DepId dep_id,
//     MarshallDeputy md_cmd
// ) :
//  Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES),slot(slot),ballot(ballot),leaderCurrentTerm(leaderCurrentTerm),leaderPrevLogIndex(leaderPrevLogIndex),leaderPrevLogTerm(leaderPrevLogTerm),leaderCommitIndex(leaderCommitIndex),dep_id(dep_id),md_cmd(md_cmd){}
//     uint64_t slot;
//     ballot_t ballot;
//     uint64_t leaderCurrentTerm;
//     uint64_t leaderPrevLogIndex;
//     uint64_t leaderPrevLogTerm;
//     uint64_t leaderCommitIndex;
//     DepId dep_id;
//     MarshallDeputy md_cmd;

//     Marshal& ToMarshal(Marshal&) const override;
//     Marshal& FromMarshal(Marshal&) override;
// };

// class AppendEntriesCommandState: public Marshallable {
//     public:
//     AppendEntriesCommandState() : Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES) {}

//     std::vector<AppendEntriesResult> results = {};

//     Marshal& ToMarshal(Marshal&) const override;
//     Marshal& FromMarshal(Marshal&) override;

// };


// } // namespace janus