#include <sstream>
#include "list.hpp"
#include "parser.hpp"
#include "environment.hpp"
#include "show.hpp"
#include <map>
#include <vector>
#include <iostream>
#include "exceptions.hpp"
#include "generic_functions.hpp"
#include "intern.hpp"

namespace sucheme{
    using std::vector;
    using std::endl;

    GCObject *eval(GCObject* a, Environment *e) {
        if(auto s = dcast<Symbol>(a))
            return env_lookup(e, s->id);
        if(auto n = dcast<Number>(a))
            return ucast(n);
        if(auto b = dcast<Bool>(a))
            return ucast(b);
        if(auto empty = dcast<Empty>(a))
            return ucast(empty);
        if(auto proc = dcast<Procedure>(a))
            return ucast(proc);
        if(auto lambdaproc = dcast<LambdaProcedure>(a))
            return ucast(lambdaproc);
        if(auto p = dcast<Pair>(a)) {
            vector<GCObject*> args = ListToVector(dcast_ex<Pair>(p->cdr));

            GCObject* ret = nullptr;
        
            //cerr << ">" << show(a) << endl << endl;

            auto f = dcast<Symbol>(p->car);
            if(f && f->id == ID_LAMBDA) {
                if(args.size() != 2) {
                    sprintf(ex_buf, "malformed_lambda argsize: expected 2 get %d", args.size());
                    throw malformed_lambda(ex_buf);
                }

                vector<int> formals;
                ListForeach(dcast_ex<Pair>(args[0]),
                            [&](GCObject* v){
                                formals.push_back(dcast_ex<Symbol>(v)->id);
                            });

                auto body = dcast_ex<Pair>(args[1]);
            
                ret = ucast(alloc<LambdaProcedure>(formals, body, e));
            } else if(f && f->id == ID_COND) {
                for(auto &i : args) {
                    vector<GCObject*> values = ListToVector(dcast_ex<Pair>(i));

                    if(values.size() != 2)
                        throw malformed_cond();

                    auto val = dcast<Symbol>(values[0]);

                    if(val && val->id == ID_ELSE)
                        ret = eval(values[1], e);
                    else if(auto val = dcast<Bool>(eval(values[0],e)))
                        if(val->value) {
                            ret = eval(values[1], e);
                            goto end;
                        }
                }
                throw not_implemented();
            } else if(f && f->id == ID_QUOTE) {
                if(args.size() != 1)
                    throw malformed_quote();

                ret = args[0];
            } else if(f && f->id == ID_DEFINE) {
                if(args.size() != 2)
                    throw malformed_define();

                if(auto symbol = dcast<Symbol>(args[0])) {
                    env_define(e, symbol->id, eval(args[1],e));
                    ret = ucast(symbol);
                }else
                    throw malformed_define();
            } else if(f && f->id == ID_DEFINE_MACRO) {
                if(args.size() != 2)
                    throw malformed_define();

                if(auto symbol = dcast<Symbol>(args[0])) {
                    auto la = dcast_ex<LambdaProcedure>(eval(args[1],e));
                    env_define(e, symbol->id, ucast(alloc<LambdaMacro>(la->formals, la->body, la->environment)));
                    ret = ucast(symbol);
                }else
                    throw malformed_define();
            } else if(f && f->id == ID_SET) {
                if(args.size() != 2) {
                    sprintf(ex_buf, "malformed_set:args expected 2 but get %d" ,args.size());
                    throw malformed_set(ex_buf);
                }
                env_set(e, dcast_ex<Symbol>(args[0])->id, eval(args[1], e));
                ret = ucast(nil());
            } else if(f && f->id == ID_BEGIN) {
                for(auto &i : args)
                    ret = eval(i, e);
            } else { // application is function call
                auto callee = eval(p->car, e);
            
                if(auto func = dcast<Procedure>(callee)) {
                    vector<GCObject*> eval_args;
            
                    for(auto &v : args)
                        eval_args.push_back(eval(v, e));

                    ret = func->call(eval_args);
                } else if(auto lambda = dcast<LambdaProcedure>(callee)) {
                    vector<GCObject*> eval_args;
            
                    for(auto &v : args)
                        eval_args.push_back(eval(v, e));

                    auto e_lambda = alloc<Environment>(e);
                
                    if(args.size() != lambda->formals.size())
                        throw not_implemented("wrong number of args");
                
                    for(unsigned int i=0; i<args.size(); i++)
                        env_define(e_lambda, lambda->formals[i], eval_args[i]);
                
                    ret = eval(ucast(lambda->body), e_lambda);
                } else if(auto lambda = dcast<LambdaMacro>(callee)) {
                    auto e_lambda = alloc<Environment>(e);
                
                    if(args.size() != lambda->formals.size())
                        throw not_implemented("wrong number of args");
                
                    for(unsigned int i=0; i<args.size(); i++)
                        env_define(e_lambda, lambda->formals[i], args[i]);
                
                    ret = eval(ucast(lambda->body), e_lambda);
                } else {
                    throw invalid_aplication("invalid_aplication:" + show(ucast(p)) + ";" + show(callee));
                }
            }
        end:
            //cerr << "<" << show(a) << " "<< show(ret) << endl << endl;
            return ret;
        }
        throw not_implemented();
    }
}
