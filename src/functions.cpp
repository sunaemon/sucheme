#include "functions.hpp"
#include <iostream>
#include "show.hpp"
#include "list.hpp"
#include "parser.hpp"
#include "eval.hpp"
#include "intern.hpp"

namespace sucheme{
    using std::vector;

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
 
    GCPtr add(const vector_ptr &arg) {
        int ret=0;
        for(auto &i : arg) 
            ret += dcast_ex<Number>(i)->integer;

        return ucast(alloc<Number>(ret));
    }

   GCPtr mul(const vector_ptr &arg) {
        int ret=1;
        for(auto &i : arg) 
            ret *= dcast_ex<Number>(i)->integer;

        return ucast(alloc<Number>(ret));
    }

   GCPtr sub(const vector_ptr &arg) {
        if(arg.size() <= 1)
            throw invalid_aplication("invalid_aplication:sub");

        int ret=dcast_ex<Number>(arg[0])->integer;

        for(auto i = arg.begin()+1; i<arg.end(); i++) 
            ret -= dcast_ex<Number>(*i)->integer;

        return ucast(alloc<Number>(ret));
    }

   GCPtr eq(const vector_ptr &arg) {
       if(arg.size() != 2)
            throw invalid_aplication("invalid_aplication:eq" + arg.size());

       return ucast(alloc<Bool>(dcast_ex<Number>(arg[0])->integer == dcast_ex<Number>(arg[1])->integer));
    }

   GCPtr lt(const vector_ptr &arg) {
       if(arg.size() != 2)
           throw invalid_aplication("invalid_aplication:lt");

       return ucast(alloc<Bool>(dcast_ex<Number>(arg[0])->integer < dcast_ex<Number>(arg[1])->integer));
    }

   GCPtr print(const vector_ptr &arg) {
        for(auto &i : arg) 
            printf(show(i).c_str());
        printf("\n");
        return ucast(alloc<Empty>());
    }

   GCPtr car(const vector_ptr &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:car" + arg.size());
       return ucast(dcast_ex<Pair>(arg[0])->car);
    }

   GCPtr cdr(const vector_ptr &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:car" + arg.size());
       return ucast(dcast_ex<Pair>(arg[0])->cdr);
    }

   GCPtr cons(const vector_ptr &arg) {
       if(arg.size() != 2)
            throw invalid_aplication("invalid_aplication:cons" + arg.size());
       return ucast(cons(arg[0], arg[1]));
    }

   GCPtr null_is(const vector_ptr &arg) {
       if(arg.size() != 1)
            throw invalid_aplication("invalid_aplication:null?" + arg.size());
       return ucast(alloc<Bool>(dcast<Empty>(arg[0])));
    }
}
