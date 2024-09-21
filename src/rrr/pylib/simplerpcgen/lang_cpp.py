from simplerpcgen.misc import SourceFile
import os

def emit_struct(struct, f):
    f.writeln("struct %s {" % struct.name)
    with f.indent():
        for field in struct.fields:
            f.writeln("%s %s;" % (field.type, field.name))
    f.writeln("};")
    f.writeln()
    f.writeln("inline rrr::Marshal& operator <<(rrr::Marshal& m, const %s& o) {" % struct.name)
    with f.indent():
        for field in struct.fields:
            f.writeln("m << o.%s;" % field.name)
        f.writeln("return m;")
    f.writeln("}")
    f.writeln()
    f.writeln("inline rrr::Marshal& operator >>(rrr::Marshal& m, %s& o) {" % struct.name)
    with f.indent():
        for field in struct.fields:
            f.writeln("m >> o.%s;" % field.name)
        f.writeln("return m;")
    f.writeln("}")
    f.writeln()

def emit_service_and_proxy(service, f, rpc_table):
    f.writeln("class %sService: public rrr::Service {" % service.name)
    f.writeln("public:")
    with f.indent():
        f.writeln("enum {")
        with f.indent():
            for func in service.functions:
                rpc_code = rpc_table["%s.%s" % (service.name, func.name)]
                f.writeln("%s = %s," % (func.name.upper(), hex(rpc_code)))
        f.writeln("};")
        f.writeln("int __reg_to__(rrr::Server* svr) {")
        with f.indent():
            f.writeln("int ret = 0;")
            for func in service.functions:
                if func.attr == "raw":
                    f.writeln("if ((ret = svr->reg(%s, this, &%sService::%s)) != 0) {" % (func.name.upper(), service.name, func.name))
                else:
                    f.writeln("if ((ret = svr->reg(%s, this, &%sService::__%s__wrapper__)) != 0) {" % (func.name.upper(), service.name, func.name))
                with f.indent():
                    f.writeln("goto err;")
                f.writeln("}")
            f.writeln("return 0;")
        f.writeln("err:")
        with f.indent():
            for func in service.functions:
                f.writeln("svr->unreg(%s);" % func.name.upper())
            f.writeln("return ret;")
        f.writeln("}")
        f.writeln("// these RPC handler functions need to be implemented by user")
        f.writeln("// for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job")
        for func in service.functions:
            if service.abstract or func.abstract:
                postfix = " = 0"
            else:
                postfix = ""
            if func.attr == "raw":
                f.writeln("virtual void %s(rrr::Request* req, rrr::ServerConnection* sconn)%s;" % (func.name, postfix))
            else:
                func_args = []
                for in_arg in func.input:
                    if in_arg.name != None:
                        func_args += "const %s& %s" % (in_arg.type, in_arg.name),
                    else:
                        func_args += "const %s&" % in_arg.type,
                for out_arg in func.output:
                    if out_arg.name != None:
                        func_args += "%s* %s" % (out_arg.type, out_arg.name),
                    else:
                        func_args += "%s*" % out_arg.type,
                if func.attr == "defer":
                    func_args += "rrr::DeferredReply* defer",
                f.writeln("virtual void %s(%s)%s;" % (func.name, ", ".join(func_args), postfix))
    f.writeln("private:")       
    with f.indent():
        for func in service.functions:
            if func.attr == "raw":
                continue
            f.writeln("void __%s__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {" % func.name)
            with f.indent():
                if func.attr == "defer":
                    invoke_with = []
                    in_counter = 0
                    out_counter = 0
                    for in_arg in func.input:
                        f.writeln("%s* in_%d = new %s;" % (in_arg.type, in_counter, in_arg.type))
                        f.writeln("req->m >> *in_%d;" % in_counter)
                        invoke_with += "*in_%d" % in_counter,
                        in_counter += 1
                    for out_arg in func.output:
                        f.writeln("%s* out_%d = new %s;" % (out_arg.type, out_counter, out_arg.type))
                        invoke_with += "out_%d" % out_counter,
                        out_counter += 1
                    f.writeln("auto __marshal_reply__ = [=] {");
                    with f.indent():
                        out_counter = 0
                        for out_arg in func.output:
                            f.writeln("*sconn << *out_%d;" % out_counter)
                            out_counter += 1
                    f.writeln("};");
                    f.writeln("auto __cleanup__ = [=] {");
                    with f.indent():
                        in_counter = 0
                        out_counter = 0
                        for in_arg in func.input:
                            f.writeln("delete in_%d;" % in_counter)
                            in_counter += 1
                        for out_arg in func.output:
                            f.writeln("delete out_%d;" % out_counter)
                            out_counter += 1
                    f.writeln("};");
                    f.writeln("rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);")
                    invoke_with += "__defer__",
                    f.writeln("this->%s(%s);" % (func.name, ", ".join(invoke_with)))
                else: # normal and fast rpc
                    if func.attr != "fast":
                        f.writeln("auto f = [=] {")
                        f.incr_indent()
                    invoke_with = []
                    in_counter = 0
                    out_counter = 0
                    for in_arg in func.input:
                        f.writeln("%s in_%d;" % (in_arg.type, in_counter))
                        f.writeln("req->m >> in_%d;" % in_counter)
                        invoke_with += "in_%d" % in_counter,
                        in_counter += 1
                    for out_arg in func.output:
                        f.writeln("%s out_%d;" % (out_arg.type, out_counter))
                        invoke_with += "&out_%d" % out_counter,
                        out_counter += 1
                    f.writeln("this->%s(%s);" % (func.name, ", ".join(invoke_with)))
                    f.writeln("sconn->begin_reply(req);")
                    for i in range(out_counter):
                        f.writeln("*sconn << out_%d;" % i)
                    f.writeln("sconn->end_reply();")
                    f.writeln("delete req;")
                    f.writeln("sconn->release();")
                    if func.attr != "fast":
                        f.decr_indent()
                        f.writeln("};")
                        f.writeln("sconn->run_async(f);")
            f.writeln("}")
    f.writeln("};")
    f.writeln()
    f.writeln("class %sProxy {" % service.name)
    f.writeln("protected:")
    with f.indent():
        f.writeln("rrr::Client* __cl__;")
    f.writeln("public:")
    with f.indent():
        f.writeln("%sProxy(rrr::Client* cl): __cl__(cl) { }" % service.name)
        for func in service.functions:
            async_func_params = []
            async_call_params = []
            sync_func_params = []
            sync_out_params = []
            in_counter = 0
            out_counter = 0
            for in_arg in func.input:
                if in_arg.name != None:
                    async_func_params += "const %s& %s" % (in_arg.type, in_arg.name),
                    async_call_params += in_arg.name,
                    sync_func_params += "const %s& %s" % (in_arg.type, in_arg.name),
                else:
                    async_func_params += "const %s& in_%d" % (in_arg.type, in_counter),
                    async_call_params += "in_%d" % in_counter,
                    sync_func_params += "const %s& in_%d" % (in_arg.type, in_counter),
                in_counter += 1
            for out_arg in func.output:
                if out_arg.name != None:
                    sync_func_params += "%s* %s" % (out_arg.type, out_arg.name),
                    sync_out_params += out_arg.name,
                else:
                    sync_func_params += "%s* out_%d" % (out_arg.type, out_counter),
                    sync_out_params += "out_%d" % out_counter,
                out_counter += 1
            f.writeln("rrr::Future* async_%s(%sconst rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {" % (func.name, ", ".join(async_func_params + [""])))
            with f.indent():
                f.writeln("rrr::Future* __fu__ = __cl__->begin_request(%sService::%s, __fu_attr__);" % (service.name, func.name.upper()))
                if len(async_call_params) > 0:
                    f.writeln("if (__fu__ != nullptr) {")
                    with f.indent():
                        for param in async_call_params:
                            f.writeln("*__cl__ << %s;" % param)
                    f.writeln("}")
                f.writeln("__cl__->end_request();")
                f.writeln("return __fu__;")
            f.writeln("}")
            f.writeln("rrr::i32 %s(%s) {" % (func.name, ", ".join(sync_func_params)))
            with f.indent():
                f.writeln("rrr::Future* __fu__ = this->async_%s(%s);" % (func.name, ", ".join(async_call_params)))
                f.writeln("if (__fu__ == nullptr) {")
                with f.indent():
                    f.writeln("return ENOTCONN;")
                f.writeln("}")
                f.writeln("rrr::i32 __ret__ = __fu__->get_error_code();")
                if len(sync_out_params) > 0:
                    f.writeln("if (__ret__ == 0) {")
                    with f.indent():
                        for param in sync_out_params:
                            f.writeln("__fu__->get_reply() >> *%s;" % param)
                    f.writeln("}")
                f.writeln("__fu__->release();")
                f.writeln("return __ret__;")
            f.writeln("}")
    f.writeln("};")
    f.writeln()

## writes the actual file contents of header file
def emit_rpc_source_cpp(rpc_source, rpc_table, fpath, cpp_header, cpp_footer):
    with open(fpath, "w") as f:
        f = SourceFile(f)
        f.writeln("#pragma once")
        f.writeln()
#        f.writeln('#include "rpc/server.h"')
        f.writeln('#include "rrr.hpp"')
        f.writeln()
        f.writeln("#include <errno.h>")
        f.writeln()
        f.write(cpp_header)
        f.writeln()

        if rpc_source.namespace != None:
            f.writeln(" ".join(map(lambda x:"namespace %s {" % x, rpc_source.namespace)))
            f.writeln()

        for struct in rpc_source.structs:
            emit_struct(struct, f)

        for service in rpc_source.services:
            emit_service_and_proxy(service, f, rpc_table)

        if rpc_source.namespace != None:
            f.writeln(" ".join(["}"] * len(rpc_source.namespace)) + " // namespace " + "::".join(rpc_source.namespace))
            f.writeln()

        f.writeln()
        f.write(cpp_footer)
        f.writeln()
                
def emit_crpc_commands(rpc_source, fpath):
    for service in rpc_source.services:
        fpath_new = fpath + service.name + "_"
        if (service.name == "FpgaRaft"): ## TODO: take service name from config
            # do the analysis here
            print("in FpgaRaft analysis")   ## TODO: take function names from config
            
            crpc_func_dict = dict()
            for func in service.functions:
                crpc_func_dict[func.name] = []
                crpc_in_args = []
                crpc_out_args = []
                for in_arg in func.input: 
                    if in_arg.name != None:
                        crpc_in_args.append("%s %s" % (in_arg.type, in_arg.name))
                for out_arg in func.output:
                    if out_arg.name != None:
                        crpc_out_args.append("%s %s" % (out_arg.type, out_arg.name))
                crpc_func_dict[func.name].append(crpc_in_args)
                crpc_func_dict[func.name].append(crpc_out_args)
            # return crpc_func_dict

            print("***** crpc_func_dict")  
            for el in crpc_func_dict:
                if el == "AppendEntries":
                    ## add the header file
                    fpath_cmd_h = fpath_new + el + "_command.h"
                    print(el)
                    print(crpc_func_dict[el][0])
                    print(crpc_func_dict[el][1])
                    print(fpath_cmd_h)
                    with open(fpath_cmd_h, "w") as f:
                        f = SourceFile(f)
                        f.writeln("#pragma once")
                        f.writeln('#include "../marshallable.h"')
                        f.writeln('#include "../rcc_rpc.h"')
                        f.writeln()
                    
                        if rpc_source.namespace != None:
                            f.writeln()
                            f.writeln(" ".join(map(lambda x:"namespace %s {" % x, rpc_source.namespace)))
                            f.writeln()                                    

                        emit_command(crpc_func_dict, el, f)
                        emit_command_state(crpc_func_dict, el, f)
                        
                        if rpc_source.namespace != None:
                            f.writeln()
                            f.writeln(" ".join(["}"] * len(rpc_source.namespace)) + " // namespace " + "::".join(rpc_source.namespace))
                            f.writeln()
                    

                    ## add the cpp file
                    fpath_cmd_cpp = fpath_new + el + "_command.cc"
                    print("#################")
                    print(fpath_cmd_cpp)
                    with open(fpath_cmd_cpp, "w") as f:
                        f = SourceFile(f)
                        f.writeln('#include "%s"' % (service.name + "_" + el + "_command.h"))
                        f.writeln()
                    
                        if rpc_source.namespace != None:
                            f.writeln()
                            f.writeln(" ".join(map(lambda x:"namespace %s {" % x, rpc_source.namespace)))
                            f.writeln()

                        emit_command_and_state_cpp(crpc_func_dict, el, f)

                        if rpc_source.namespace != None:
                            f.writeln()
                            f.writeln(" ".join(["}"] * len(rpc_source.namespace)) + " // namespace " + "::".join(rpc_source.namespace))
                            f.writeln()

    
def emit_command(crpc_func_dict, el, f):
    ## function_command class
    class_name = str(el)+"Command"
    f.writeln("class %s: public Marshallable {" % class_name)
    
    with f.indent():
        f.writeln("public:")
        ## first constructor
        f.writeln("%s() : Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES) {}" % class_name) # TODO: get the cmd_kind from config?
        f.writeln()

        ## first constructor
        f.writeln("%s(" % class_name)
        ip_len = len(crpc_func_dict[el][0])
        for index, ip in enumerate(crpc_func_dict[el][0]):
            if index < ip_len-1:
                f.writeln("%s," % ip)
            else:
                f.writeln("%s" % ip)
        f.write(") :\n Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES),")
        for index, ip in enumerate(crpc_func_dict[el][0]):
            if index < ip_len -1:
                f.write("%s(%s)," % (ip.split()[1], ip.split()[1]))
            else:
                f.write("%s(%s)" % (ip.split()[1], ip.split()[1]))
        f.write("{}")
        f.writeln()
        
        ## declare the class variables
        for index, ip in enumerate(crpc_func_dict[el][0]):
            f.writeln("%s;" % (ip))

        f.writeln()
        
        ## toMarshal and fromMarshal methods declaration
        f.writeln("Marshal& ToMarshal(Marshal&) const override;")
        f.writeln("Marshal& FromMarshal(Marshal&) override;")


    f.writeln("};") ## end of cmd class
    f.writeln()    

    ## end of function_command class


def emit_command_state(crpc_func_dict, el, f):
    ## function_command_state class
    class_name = str(el)+"CommandState"
    f.writeln("class %s: public Marshallable {" % class_name)
    
    with f.indent():
        f.writeln("public:")
        f.writeln("%s() : Marshallable(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES) {}" % class_name) # TODO: get the cmd_kind from config?
        f.writeln()
        
        f.writeln("std::vector<%s> results = {};" % (str(el)+"Result"))
        f.writeln()

        ## toMarshal and fromMarshal methods declaration
        f.writeln("Marshal& ToMarshal(Marshal&) const override;")
        f.writeln("Marshal& FromMarshal(Marshal&) override;")
        f.writeln()
  
    f.writeln("};")
    f.writeln()

def emit_command_and_state_cpp(crpc_func_dict, el, f):
    f.writeln()
    f.writeln("static int volatile x1 =")
    f.writeln("MarshallDeputy::RegInitializer(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES,") # TODO: add cmd from config
    f.writeln("[] () -> Marshallable* {return new %sCommand;});" % (str(el)))
    f.writeln()

    f.writeln("static int volatile x2 =")
    f.writeln("MarshallDeputy::RegInitializer(MarshallDeputy::CMD_RAFT_APPEND_ENTRIES_STATE,") # TODO: add cmd from config
    f.writeln("[] () -> Marshallable* {return new %sCommandState;});" % (str(el)))
    f.writeln()

    class_name = str(el)+"Command"
    ## toMarshal method
    f.writeln("Marshal& %s::ToMarshal(Marshal& m) const{" % (class_name))
    f.incr_indent()
    for index, ip in enumerate(crpc_func_dict[el][0]):
        f.writeln("m << %s;" % (ip.split()[1]))
    f.writeln("return m;")
    f.decr_indent()
    f.writeln("}")
    ## toMarshal method end

    f.writeln()
    
    ## fromMarshal method
    f.writeln("Marshal& %s::FromMarshal(Marshal& m) {" % (class_name))
    f.incr_indent()
    for index, ip in enumerate(crpc_func_dict[el][0]):
        f.writeln("m >> %s;" % (ip.split()[1]))
    f.writeln("return m;")
    f.decr_indent()
    f.writeln("}")
    ## fromMarshal method end 



    class_name = str(el)+"CommandState"
     ## below implements the function, just define it for now
    f.writeln("Marshal& %s::ToMarshal(Marshal& m) const{" % (class_name))
    f.incr_indent()
    f.writeln("m << results;")
    f.writeln("return m;")
    f.decr_indent()
    f.writeln("}")
    ## toMarshal method end      
    
    f.writeln()
    
    ## fromMarshal method
    f.writeln("Marshal& %s::FromMarshal(Marshal& m) {" % (class_name))
    f.incr_indent()
    f.writeln("m >> results;")
    f.writeln("return m;")
    f.decr_indent()
    f.writeln("}")
    ## fromMarshal method end 