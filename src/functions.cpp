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
 
    GCPtr add(unsigned int argc, const GCPtr argv[]) {
        int ret=0;
        for(unsigned int i=0; i<argc; i++)
            ret += dcast_ex<Number>(argv[i])->integer;

        return ucast(alloc<Number>(ret));
    }

   GCPtr mul(unsigned int argc, const GCPtr argv[]) {
        int ret=1;
        for(unsigned int i=0; i<argc; i++)
            ret *= dcast_ex<Number>(argv[i])->integer;

        return ucast(alloc<Number>(ret));
    }

   GCPtr sub(unsigned int argc, const GCPtr argv[]) {
        if(argc <= 1)
            throw invalid_aplication("invalid_aplication:sub");

        int ret=dcast_ex<Number>(argv[0])->integer;

        for(unsigned int i=1; i<argc; i++)
            ret -= dcast_ex<Number>(argv[i])->integer;

        return ucast(alloc<Number>(ret));
    }

   GCPtr eq(unsigned int argc, const GCPtr argv[]) {
       if(argc != 2)
            throw invalid_aplication("invalid_aplication:eq" + argc);

       return ucast(alloc<Bool>(dcast_ex<Number>(argv[0])->integer == dcast_ex<Number>(argv[1])->integer));
    }

   GCPtr lt(unsigned int argc, const GCPtr argv[]) {
       if(argc != 2)
           throw invalid_aplication("invalid_aplication:lt");

       return ucast(alloc<Bool>(dcast_ex<Number>(argv[0])->integer < dcast_ex<Number>(argv[1])->integer));
    }

   GCPtr print(unsigned int argc, const GCPtr argv[]) {
        for(unsigned int i=0; i<argc; i++)
            printf(show(argv[i]).c_str());
        printf("\n");
        return ucast(alloc<Empty>());
    }

   GCPtr car(unsigned int argc, const GCPtr argv[]) {
       if(argc != 1)
            throw invalid_aplication("invalid_aplication:car" + argc);
       return ucast(dcast_ex<Pair>(argv[0])->car);
    }

   GCPtr cdr(unsigned int argc, const GCPtr argv[]) {
       if(argc != 1)
            throw invalid_aplication("invalid_aplication:car" + argc);
       return ucast(dcast_ex<Pair>(argv[0])->cdr);
    }

   GCPtr cons(unsigned int argc, const GCPtr argv[]) {
       if(argc != 2)
            throw invalid_aplication("invalid_aplication:cons" + argc);
       return ucast(cons(argv[0], argv[1]));
    }

   GCPtr null_is(unsigned int argc, const GCPtr argv[]) {
       if(argc != 1)
            throw invalid_aplication("invalid_aplication:null?" + argc);
       return ucast(alloc<Bool>(dcast<Empty>(argv[0])));
    }
}
