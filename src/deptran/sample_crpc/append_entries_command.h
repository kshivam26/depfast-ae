#pragma once
#include "../marshallable.h"
#include "../procedure.h"
#include "../rcc/tx.h"
#include "../rcc_rpc.h"


namespace janus{

class AppendEntriesCommandToyCrpc : public Marshallable {
    public:
    AppendEntriesCommandToyCrpc() : Marshallable(MarshallDeputy::CMD_SAMPLE_CRPC_APPEND_ENTRIES) {
    }
  
    AppendEntriesCommandToyCrpc(MarshallDeputy md_cmd) : 
        Marshallable(MarshallDeputy::CMD_SAMPLE_CRPC_APPEND_ENTRIES),
        md_cmd(md_cmd) {}
    MarshallDeputy md_cmd;

    Marshal& ToMarshal(Marshal&) const override;
    Marshal& FromMarshal(Marshal&) override;
};

class AppendEntriesCommandStateToyCrpc : public Marshallable {
    public:
    AppendEntriesCommandStateToyCrpc() : Marshallable(MarshallDeputy::CMD_SAMPLE_CRPC_APPEND_ENTRIES_STATE) {
    }

    std::vector<AppendEntriesAdd> results = {};
    Marshal& ToMarshal(Marshal&) const override;
    Marshal& FromMarshal(Marshal&) override;
};

}