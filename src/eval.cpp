#include <sstream>
#include "parser.hpp"
#include "environment.hpp"
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
    using std::cerr;
    
    template<typename T>
    void ListForeach(const shared_ptr<Pair> &list, T callback) {
        Pair *l = list.get();
        Pair *ll;

        for(;;){
            callback(l->car);
            ll = dynamic_cast<Pair*>(l->cdr.get());
            if(!ll) break;
            l = ll;
        }

        if(typeid(*l->cdr.get()) != typeid(Empty))
            throw improper_list();
    }
 
    shared_ptr<LispVal> Symbol::eval(shared_ptr<Environment> &e) {
        return e->lookup(name);
    }

    shared_ptr<LispVal> Pair::eval(shared_ptr<Environment> &e) {
        vector<shared_ptr<LispVal> > args;
        
        std::cerr  << "eval " << show() << " ";

        ListForeach(dcast<Pair>(cdr),
                    [&args, &e](const shared_ptr<LispVal> &v){
                        args.push_back(v);
                    });

        auto f = dynamic_pointer_cast<Symbol>(car);
        if(f && f->name == "lambda") { cerr << "lambda" << endl;
            if(args.size() != 2)
                throw malformed_lambda();

            vector<string> formals;
            ListForeach(dcast<Pair>(args[0]),
                        [&](const shared_ptr<LispVal> &v){
                            formals.push_back(dcast<Symbol>(v)->name);
                        });

            auto body = dcast<Pair>(args[1]);
            
            return make_shared<LambdaProcedure>(formals, body, e);
        } else if(f && f->name == "cond") { cerr << "cond" << endl;
            throw not_implemented();
        } else if(f && f->name == "define") { cerr << "define" << endl;
            if(args.size() != 2)
                throw malformed_define();

            if(auto symbol = dynamic_pointer_cast<Symbol>(args[0])) {
                e->define(symbol->name, args[1]);
                return symbol;
            } else if(auto func = dynamic_pointer_cast<Pair>(args[0])) {
                vector<string> formals;
                auto symbol = dcast<Symbol>(func->car);
        
                ListForeach(dcast<Pair>(dcast<Pair>(args[0])->cdr),
                            [&formals, &e](const shared_ptr<LispVal> &v){
                                formals.push_back(dcast<Symbol>(v)->name);
                            });
                
                e->define(symbol->name, make_shared<LambdaProcedure>(formals, dcast<Pair>(args[1]), e));
                return symbol;
            } else
                throw malformed_define();
        } else { // application is function call
            cerr << "function call" << endl;
            vector<shared_ptr<LispVal> > eval_args;
            
            for(auto &v : args)
                eval_args.push_back(v->eval(e));
            
            auto callee = car->eval(e);
            
            if(auto func = dynamic_pointer_cast<Procedure>(callee)) {
                return func->call(eval_args);
            } else if(auto lambda = dynamic_pointer_cast<LambdaProcedure>(callee)) {
                auto e_lambda = make_shared<Environment>();
                e_lambda->parent = e;
                
                if(args.size() != lambda->formals.size())
                    throw not_implemented("wrong number of args");
                
                for(unsigned int i=0; i<args.size(); i++)
                    e_lambda->define(lambda->formals[i], eval_args[i]);
                
                return lambda->body->eval(e_lambda);
            } else
                throw invaild_aplication("invaild_aplication:" + show());
        }
    }
}
