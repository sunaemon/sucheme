#include "functions.hpp"
#include "show.hpp"
#include "list.hpp"
#include "parser.hpp"
#include "eval.hpp"
#include "intern.hpp"


    void init_environment(Environment *e)
    {
        env_intern_define(e, "+", ucast(alloc<Procedure>(add)));
        env_intern_define(e, "=", ucast(alloc<Procedure>(eq)));
        env_intern_define(e, "*", ucast(alloc<Procedure>(mul)));
        env_intern_define(e, "-", ucast(alloc<Procedure>(sub)));
        env_intern_define(e, "car", ucast(alloc<Procedure>(car)));
        env_intern_define(e, "cdr", ucast(alloc<Procedure>(cdr)));
        env_intern_define(e, "print", ucast(alloc<Procedure>(print)));
        env_intern_define(e, "null?", ucast(alloc<Procedure>(null_is)));
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
       if(argc <= 1) {
            sprintf(ex_buf, "invalid_aplication:sub");
            throw_jump();
       }

       int ret=dcast_ex<Number>(argv[0])->integer;
       
       for(unsigned int i=1; i<argc; i++)
           ret -= dcast_ex<Number>(argv[i])->integer;
       
       return ucast(alloc<Number>(ret));
   }

   GCPtr eq(unsigned int argc, const GCPtr argv[]) {
       if(argc != 2) {
           sprintf(ex_buf, "invalid_aplication:eq %d", argc);
           throw_jump();
       }

       return ucast(alloc<Bool>(dcast_ex<Number>(argv[0])->integer == dcast_ex<Number>(argv[1])->integer));
    }

   GCPtr lt(unsigned int argc, const GCPtr argv[]) {
       if(argc != 2) {
           sprintf(ex_buf, "invalid_aplication:lt %d", argc);
           throw_jump();
       }

       return ucast(alloc<Bool>(dcast_ex<Number>(argv[0])->integer < dcast_ex<Number>(argv[1])->integer));
    }

   GCPtr print(unsigned int argc, const GCPtr argv[]) {
       for(unsigned int i=0; i<argc; i++) {
           char *buf = show(argv[i]);
           puts(buf);
           free(buf);
       }
       printf("\n");
       return ucast(alloc<Empty>());
    }

   GCPtr car(unsigned int argc, const GCPtr argv[]) {
       if(argc != 1) {
           sprintf(ex_buf, "invalid_aplication:car %d", argc);
           throw_jump();
       }
       return ucast(dcast_ex<Pair>(argv[0])->car);
    }

   GCPtr cdr(unsigned int argc, const GCPtr argv[]) {
       if(argc != 1) {
           sprintf(ex_buf, "invalid_aplication:cdr %d", argc);
           throw_jump();
       }
       return ucast(dcast_ex<Pair>(argv[0])->cdr);
    }

   GCPtr cons(unsigned int argc, const GCPtr argv[]) {
       if(argc != 2) {
           sprintf(ex_buf, "invalid_aplication:cons %d", argc);
           throw_jump();
       }
       return ucast(cons(argv[0], argv[1]));
   }

GCPtr null_is(unsigned int argc, const GCPtr argv[]) {
    if(argc != 1) {
        sprintf(ex_buf, "invalid_aplication:null? %d", argc);
        throw_jump();
    }
    return ucast(alloc<Bool>(dcast<Empty>(argv[0])));
}
