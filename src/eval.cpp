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

    vector<shared_ptr<LispVal> > ListToVector(const shared_ptr<Pair> &list) {
        vector<shared_ptr<LispVal> > ret;
        ListForeach(list,[&ret](const shared_ptr<LispVal> &v){ret.push_back(v);});
        return ret;
    }

 
    shared_ptr<LispVal> Symbol::eval(shared_ptr<Environment> &e) {
        return e->lookup(name);
    }

    shared_ptr<LispVal> Pair::eval(shared_ptr<Environment> &e) {
        vector<shared_ptr<LispVal> > args = ListToVector(dcast<Pair>(cdr));

        shared_ptr<LispVal> ret;
        
        //cerr << show() << endl;

        auto f = dynamic_pointer_cast<Symbol>(car);
        if(f && f->name == "lambda") {
            if(args.size() != 2)
                throw malformed_lambda();

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
                    ret = values[1]->eval(e);
                else if(auto val = dynamic_pointer_cast<Bool>(values[0]->eval(e)))
                    if(val->value) {
                        ret = values[1]->eval(e);
                        goto end;
                    }
            }
            throw not_implemented();
        } else if(f && f->name == "define") {
            if(args.size() != 2)
                throw malformed_define();

            if(auto symbol = dynamic_pointer_cast<Symbol>(args[0])) {
                e->define(symbol->name, args[1]);
                ret = symbol;
            }else
                throw malformed_define();
        } else if(f && f->name == "letrec") {
            if(args.size() != 2)
                throw malformed_letrec();

            auto e_letrec = make_shared<Environment>(e);

            ListForeach(dcast<Pair>(args[0]),
                        [&](const shared_ptr<LispVal> &v) {
                            vector<shared_ptr<LispVal> > binding_spec = ListToVector(dcast<Pair>(v));

                            if(binding_spec.size() != 2)
                                throw malformed_letrec();
                            
                            e_letrec->define(dcast<Symbol>(binding_spec[0])->name, binding_spec[1]->eval(e_letrec));
                        });
            ret = args[1]->eval(e_letrec);
        } else { // application is function call
            vector<shared_ptr<LispVal> > eval_args;
            
            for(auto &v : args)
                eval_args.push_back(v->eval(e));
            
            auto callee = car->eval(e);
            
            if(auto func = dynamic_pointer_cast<Procedure>(callee)) {
                ret = func->call(eval_args);
            } else if(auto lambda = dynamic_pointer_cast<LambdaProcedure>(callee)) {
                auto e_lambda = make_shared<Environment>(e);
                
                if(args.size() != lambda->formals.size())
                    throw not_implemented("wrong number of args");
                
                for(unsigned int i=0; i<args.size(); i++)
                    e_lambda->define(lambda->formals[i], eval_args[i]);
                
                ret = lambda->body->eval(e_lambda);
            } else
                throw invalid_aplication("invalid_aplication:" + show() + ";" + callee->show());
        }
    end:
        //cerr << show() << " "<< ret->show() << endl;
        return ret;
    }
}
