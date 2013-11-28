#include "functions.hpp"
#include <iostream>
#include "show.hpp"
#include "list.hpp"
#include "parser.hpp"
#include "eval.hpp"
#include "intern.hpp"

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::cout;
    using std::endl;

    void init_environment(Environment *e)
    {
        env_intern_define(e, "+", ucast(alloc<Procedure>(sucheme::add)));
        env_intern_define(e, "=", ucast(alloc<Procedure>(sucheme::eq)));
        env_intern_define(e, "*", ucast(alloc<Procedure>(sucheme::mul)));
        env_intern_define(e, "-", ucast(alloc<Procedure>(sucheme::sub)));
        env_intern_define(e, "car", ucast(alloc<Procedure>(sucheme::car)));
        env_intern_define(e, "cdr", ucast(alloc<Procedure>(sucheme::cdr)));
        env_intern_define(e, "print", ucast(alloc<Procedure>(sucheme::print)));
        env_intern_define(e, "null?", ucast(alloc<Procedure>(sucheme::null_is)));
        env_intern_define(e, "else", ucast(alloc<Bool>(true)));
        
        eval(parse("(define cadr (lambda (x) (car (cdr x))))"),e);
        eval(parse("(define cdar (lambda (x) (cdr (car x))))"),e);
        eval(parse("(define caar (lambda (x) (car (car x))))"),e);
        eval(parse("(define cddr (lambda (x) (cdr (cdr x))))"),e);
    }
 
    GCObject* add(const vector<GCObject*> &arg) {
        int ret=0;
        for(auto &i : arg) 
            ret += dcast_ex<Number>(i)->integer;

        return ucast(alloc<Number>(ret));
    }

   GCObject* mul(const vector<GCObject*> &arg) {
        int ret=1;
        for(auto &i : arg) 
            ret *= dcast_ex<Number>(i)->integer;

        return ucast(alloc<Number>(ret));
    }

   GCObject* sub(const vector<GCObject*> &arg) {
        if(arg.size() <= 1)
            throw invalid_aplication("invalid_aplication:sub");

        int ret=dcast_ex<Number>(arg[0])->integer;

        for(auto i = arg.begin()+1; i<arg.end(); i++) 
            ret -= dcast_ex<Number>(*i)->integer;

        return ucast(alloc<Number>(ret));
    }

   GCObject* eq(const vector<GCObject*> &arg) {
       if(arg.size() != 2)
            throw invalid_aplication("invalid_aplication:eq" + arg.size());

       return ucast(alloc<Bool>(dcast_ex<Number>(arg[0])->integer == dcast_ex<Number>(arg[1])->integer));
    }

   GCObject* lt(const vector<GCObject*> &arg) {
       if(arg.size() != 2)
           throw invalid_aplication("invalid_aplication:lt");

       return ucast(alloc<Bool>(dcast_ex<Number>(arg[0])->integer < dcast_ex<Number>(arg[1])->integer));
    }

   GCObject* print(const vector<GCObject*> &arg) {
        for(auto &i : arg) 
            cout << show(i);
        cout << endl;
        return ucast(alloc<Empty>());
    }

   GCObject* car(const vector<GCObject*> &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:car" + arg.size());
       return ucast(dcast_ex<Pair>(arg[0])->car);
    }

   GCObject* cdr(const vector<GCObject*> &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:car" + arg.size());
       return ucast(dcast_ex<Pair>(arg[0])->cdr);
    }

   GCObject* cons(const vector<GCObject*> &arg) {
       if(arg.size() != 2)
            throw invalid_aplication("invalid_aplication:cons" + arg.size());
       return ucast(cons(arg[0], arg[1]));
    }

   GCObject* null_is(const vector<GCObject* > &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:null?" + arg.size());
       return ucast(alloc<Bool>(dcast<Empty>(arg[0])));
    }
}
