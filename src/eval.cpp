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

    shared_ptr<LispVal> eval(const shared_ptr<LispVal> &a, shared_ptr<Environment> &e) {
        if(auto num = dynamic_pointer_cast<Number>(a))
            return num->shared_from_this();
        if(auto b = dynamic_pointer_cast<Bool>(a))
            return b->shared_from_this();
        if(auto symbol = dynamic_pointer_cast<Symbol>(a))
            return e->lookup(symbol->name);
        if(auto p = dynamic_pointer_cast<Pair>(a)) {
            vector<shared_ptr<LispVal> > args = ListToVector(dcast<Pair>(p->cdr));

            shared_ptr<LispVal> ret;
        
            //cerr << ">" << show(a) << endl << endl;

            auto f = dynamic_pointer_cast<Symbol>(p->car);
            if(f && f->name == "lambda") {
                if(args.size() != 2)
                    throw malformed_lambda("malformed_lambda argsize: expected 2 get " + to_string(args.size()));

                vector<string> formals;
                ListForeach(dcast<Pair>(args[0]),
                            [&](const shared_ptr<LispVal> &v){
                                formals.push_back(dcast<Symbol>(v)->name);
                            });

                auto body = dcast<Pair>(args[1]);
            
                ret = make_shared<LambdaProcedure>(formals, body, e);
            } else if(f && f->name == "cond") {
                for(auto &i : args) {
                    vector<shared_ptr<LispVal> > values = ListToVector(dcast<Pair>(i));

                    if(values.size() != 2)
                        throw malformed_cond();

                    auto val = dynamic_pointer_cast<Symbol>(values[0]);

                    if(val && val->name == "else")
                        ret = eval(values[1], e);
                    else if(auto val = dynamic_pointer_cast<Bool>(eval(values[0],e)))
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

                if(auto symbol = dynamic_pointer_cast<Symbol>(args[0])) {
                    e->define(symbol->name, eval(args[1],e));
                    ret = symbol;
                }else
                    throw malformed_define();
            } else if(f && f->name == "define-macro") {
                if(args.size() != 2)
                    throw malformed_define();

                if(auto symbol = dynamic_pointer_cast<Symbol>(args[0])) {
                    auto la = dcast<LambdaProcedure>(eval(args[1],e));
                    e->define(symbol->name, make_shared<LambdaMacro>(la->formals, la->body, la->environment));
                    ret = symbol;
                }else
                    throw malformed_define();
            } else if(f && f->name == "set!") {
                if(args.size() != 2)
                    throw malformed_set("malformed_set:args expected 2 but get " + to_string(args.size()));
                e->set(dcast<Symbol>(args[0])->name, eval(args[1], e));
                ret = nil();
            } else if(f && f->name == "begin") {
                for(auto &i : args)
                    ret = eval(i, e);
            } else { // application is function call
            
                auto callee = eval(p->car, e);
            
                if(auto func = dynamic_pointer_cast<Procedure>(callee)) {
                    vector<shared_ptr<LispVal> > eval_args;
            
                    for(auto &v : args)
                        eval_args.push_back(eval(v, e));

                    ret = func->call(eval_args);
                } else if(auto lambda = dynamic_pointer_cast<LambdaProcedure>(callee)) {
                    vector<shared_ptr<LispVal> > eval_args;
            
                    for(auto &v : args)
                        eval_args.push_back(eval(v, e));

                    auto e_lambda = make_shared<Environment>(e);
                
                    if(args.size() != lambda->formals.size())
                        throw not_implemented("wrong number of args");
                
                    for(unsigned int i=0; i<args.size(); i++)
                        e_lambda->define(lambda->formals[i], eval_args[i]);
                
                    ret = eval(lambda->body, e_lambda);
                } else if(auto lambda = dynamic_pointer_cast<LambdaMacro>(callee)) {
                    auto e_lambda = make_shared<Environment>(e);
                
                    if(args.size() != lambda->formals.size())
                        throw not_implemented("wrong number of args");
                
                    for(unsigned int i=0; i<args.size(); i++)
                        e_lambda->define(lambda->formals[i], args[i]);
                
                    ret = eval(lambda->body, e_lambda);
                } else
                    throw invalid_aplication("invalid_aplication:" + show(p) + ";" + show(callee));
            }
        end:
            //cerr << "<" << show(a) << " "<< show(ret) << endl << endl;
            return ret;
        }
        if(auto empty = dynamic_pointer_cast<Empty>(a))
            return empty->shared_from_this();
        if(auto proc = dynamic_pointer_cast<Procedure>(a))
            return proc->shared_from_this();
        if(auto lambdaproc = dynamic_pointer_cast<LambdaProcedure>(a))
            return lambdaproc->shared_from_this();
        throw not_implemented();
    }
}
