#include "list.h"
#include "parser.h"
#include "environment.h"
#include "show.h"
#include "exceptions.h"
#include "intern.h"
#include "gc_objects.h"

GCPtr eval(GCPtr a, Environment *e) {
    Symbol *s;
    Number *n;
    Bool *b;
    Empty *empty;
    Procedure *proc;
    LambdaProcedure *lambdaproc;
    Pair *p;
    if((s = dcast_Symbol(a)))
        return env_lookup(e, s->id);
    if((n = dcast_Number(a)))
        return ucast(n);
    if((b = dcast_Bool(a)))
        return ucast(b);
    if((empty = dcast_Empty(a)))
        return ucast(empty);
    if((proc = dcast_Procedure(a)))
        return ucast(proc);
    if((lambdaproc = dcast_LambdaProcedure(a)))
        return ucast(lambdaproc);
    if((p = dcast_Pair(a))) {
        GCPtr args[LAMBDA_MAX_ARG];
        unsigned int argc = ListToArray(args, dcast_ex_Pair(p->cdr));

        GCPtr ret = NULL;
        
        Symbol *f = dcast_Symbol(p->car);

        if(f && f->id == ID_LAMBDA) {
            if(argc != 2)
                throw_jumpf("malformed_lambda argsize: expected 2 get %d", argc);

            GCPtr body = args[1];
            LambdaProcedure *lp = alloc_LambdaProcedure(body, e);
            int i=0;

            Pair *next = dcast_ex_Pair(args[0]);
            for(; i<LAMBDA_MAX_ARG; i++) {
                lp->argv[i++] = dcast_ex_Symbol(next->car)->id;
                
                Pair *tmp;
                if(!(tmp = dcast_Pair(next->cdr)))
                    break;
                next = tmp;
            }

            if(i==LAMBDA_MAX_ARG)
                throw_jumpf("too_many_argument");
            
            if(!dcast_Empty(next->cdr))
                throw_jumpf("improper_list");

            lp->argc = i;
            ret = ucast(lp);
        } else if(f && f->id == ID_COND) {
            ret = (GCPtr)nil();

            for(unsigned int i=0; i<argc; i++) {
                GCPtr values[LAMBDA_MAX_ARG];
                unsigned int values_size = ListToArray(values, dcast_ex_Pair(args[i]));

                if(values_size != 2)
                    throw_jumpf("malformed_cond");

                Bool *b_val;

                if((b_val = dcast_Bool(eval(values[0],e))))
                    if(b_val->value) {
                        ret = eval(values[1], e);
                        goto end;
                    }
            }
        } else if(f && f->id == ID_QUOTE) {
            if(argc != 1)
                throw_jumpf("malformed_quote");

            ret = args[0];
        } else if(f && f->id == ID_DEFINE) {
            if(argc != 2)
                throw_jumpf("malformed_define");

            Symbol *symbol;

            if((symbol = dcast_Symbol(args[0]))) {
                env_define(e, symbol->id, eval(args[1],e));
                ret = ucast(symbol);
            } else {
                throw_jumpf("not_implemented");
            }
        } else if(f && f->id == ID_SET) {
            if(argc != 2)
                throw_jumpf("malformed_set:args expected 2 but get %d" ,argc);

            env_set(e, dcast_ex_Symbol(args[0])->id, eval(args[1], e));
            ret = ucast(nil());
        } else if(f && f->id == ID_BEGIN) {
            for(unsigned int i=0; i<argc; i++)
                ret = eval(args[i], e);
        } else { // application is function call
            GCPtr callee = eval(p->car, e);
            Procedure *func;
            LambdaProcedure *lambda;
            if((func = dcast_Procedure(callee))) {
                GCPtr eval_args[LAMBDA_MAX_ARG];

                int eval_argc=0;

                for(unsigned int i=0; i<argc; i++)
                    eval_args[eval_argc++] = eval(args[i], e);

                ret = func->func(eval_argc, eval_args);
            } else if((lambda = dcast_LambdaProcedure(callee))) {
                GCPtr eval_args[LAMBDA_MAX_ARG];
            
                for(unsigned int i=0; i<argc; i++)
                    eval_args[i] = eval(args[i], e);

                Environment *e_lambda = alloc_Environment(e);

                if(argc != (unsigned int)lambda->argc)
                    throw_jumpf("not_implemented: wrong number of args");
                
                for(unsigned int i=0; i<argc; i++)
                    env_define(e_lambda, lambda->argv[i], eval_args[i]);
                
                ret = eval(ucast(lambda->body), e_lambda);
            } else {
                char *p_buf = show(ucast(p));
                char *callee_buf = show(callee);
                sprintf(ex_buf, "invalid_aplication: %s; %s", p_buf, callee_buf);
                free(p_buf);
                free(callee_buf);
                throw_jump();
            }
        }
    end:
#ifdef EVAL_DEBUG
        cerr << "<" << show(a) << " "<< show(ret) << endl << endl;
#endif
        return ret;
    }
    throw_jumpf("not_implemented:%s", tag_name[a->tag]);
}
