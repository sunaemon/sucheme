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

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::unique_ptr;
    using std::dynamic_pointer_cast;
    using std::endl;

    LispVal *eval(LispVal* a, Environment *e) {
        if(auto symbol = dynamic_cast<Symbol*>(a))
            return env_lookup(e, symbol->name);
        if(auto num = dynamic_cast<Number*>(a))
            return num;
        if(auto p = dynamic_cast<Pair*>(a)) {
            vector<LispVal*> args = ListToVector(dcast_ex<Pair>(p->cdr));

            LispVal* ret = nullptr;
        
            //cerr << ">" << show(a) << endl << endl;

            auto f = dynamic_cast<Symbol*>(p->car);
            if(f && f->name == "lambda") {
                if(args.size() != 2)
                    throw malformed_lambda("malformed_lambda argsize: expected 2 get " + to_string(args.size()));

                vector<string> formals;
                ListForeach(dcast_ex<Pair>(args[0]),
                            [&](LispVal* v){
                                formals.push_back(dcast_ex<Symbol>(v)->name);
                            });

                auto body = dcast_ex<Pair>(args[1]);
            
                ret = alloc<LambdaProcedure>(formals, body, e);
            } else if(f && f->name == "cond") {
                for(auto &i : args) {
                    vector<LispVal*> values = ListToVector(dcast_ex<Pair>(i));

                    if(values.size() != 2)
                        throw malformed_cond();

                    auto val = dynamic_cast<Symbol*>(values[0]);

                    if(val && val->name == "else")
                        ret = eval(values[1], e);
                    else if(auto val = dynamic_cast<Bool*>(eval(values[0],e)))
                        if(val->value) {
                            ret = eval(values[1], e);
                            goto end;
                        }
                }
                throw not_implemented();
            } else if(f && f->name == "quote") {
                if(args.size() != 1)
                    throw malformed_quote();

                ret = args[0];
            } else if(f && f->name == "define") {
                if(args.size() != 2)
                    throw malformed_define();

                if(auto symbol = dynamic_cast<Symbol*>(args[0])) {
                    env_define(e, symbol->name, eval(args[1],e));
                    ret = symbol;
                }else
                    throw malformed_define();
            } else if(f && f->name == "define-macro") {
                if(args.size() != 2)
                    throw malformed_define();

                if(auto symbol = dynamic_cast<Symbol*>(args[0])) {
                    auto la = dcast_ex<LambdaProcedure>(eval(args[1],e));
                    env_define(e, symbol->name, alloc<LambdaMacro>(la->formals, la->body, la->environment));
                    ret = symbol;
                }else
                    throw malformed_define();
            } else if(f && f->name == "set!") {
                if(args.size() != 2)
                    throw malformed_set("malformed_set:args expected 2 but get " + to_string(args.size()));
                env_set(e, dcast_ex<Symbol>(args[0])->name, eval(args[1], e));
                ret = nil();
            } else if(f && f->name == "begin") {
                for(auto &i : args)
                    ret = eval(i, e);
            } else { // application is function call
            
                auto callee = eval(p->car, e);
            
                if(auto func = dynamic_cast<Procedure*>(callee)) {
                    vector<LispVal*> eval_args;
            
                    for(auto &v : args)
                        eval_args.push_back(eval(v, e));

                    ret = func->call(eval_args);
                } else if(auto lambda = dynamic_cast<LambdaProcedure*>(callee)) {
                    vector<LispVal*> eval_args;
            
                    for(auto &v : args)
                        eval_args.push_back(eval(v, e));

                    auto e_lambda = alloc<Environment>(e);
                
                    if(args.size() != lambda->formals.size())
                        throw not_implemented("wrong number of args");
                
                    for(unsigned int i=0; i<args.size(); i++)
                        env_define(e_lambda, lambda->formals[i], eval_args[i]);
                
                    ret = eval(lambda->body, e_lambda);
                } else if(auto lambda = dynamic_cast<LambdaMacro*>(callee)) {
                    auto e_lambda = alloc<Environment>(e);
                
                    if(args.size() != lambda->formals.size())
                        throw not_implemented("wrong number of args");
                
                    for(unsigned int i=0; i<args.size(); i++)
                        env_define(e_lambda, lambda->formals[i], args[i]);
                
                    ret = eval(lambda->body, e_lambda);
                } else
                    throw invalid_aplication("invalid_aplication:" + show(p) + ";" + show(callee));
            }
        end:
            //cerr << "<" << show(a) << " "<< show(ret) << endl << endl;
            return ret;
        }
        if(auto b = dynamic_cast<Bool*>(a))
            return b;
        if(auto empty = dynamic_cast<Empty*>(a))
            return empty;
        if(auto proc = dynamic_cast<Procedure*>(a))
            return proc;
        if(auto lambdaproc = dynamic_cast<LambdaProcedure*>(a))
            return lambdaproc;
        throw not_implemented();
    }
}
