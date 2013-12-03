#include "list.h"
#include "parser.h"
#include "environment.h"
#include "show.h"
#include "exceptions.h"
#include "generic_functions.h"
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
            if(argc != 2) {
                sprintf(ex_buf, "malformed_lambda argsize: expected 2 get %d", argc);
                longjmp(ex_jbuf,0);
            }

            GCPtr body = args[1];
            LambdaProcedure *lp = alloc_LambdaProcedure(body, e);
            int i=0;
            {
                Pair *l_ = dcast_ex_Pair(args[0]);
                Pair *ll_;
                
                for(;;){
                    if(i<LAMBDA_MAX_ARG)
                        lp->argv[i++] = dcast_ex_Symbol(l_->car)->id;
                    else {
                        sprintf(ex_buf, "too_many_argument");
                        throw_jump();
                    }

                    ll_ = dcast_Pair(l_->cdr);
                    if(!ll_) break;
                    l_ = ll_;
                }
                
                if(!dcast_Empty(l_->cdr)) {
                    sprintf(ex_buf, "improper_list");
                    throw_jump();
                }
            }
            lp->argc = i;
            ret = ucast(lp);
        } else if(f && f->id == ID_COND) {
            for(unsigned int i=0; i<argc; i++) {
                GCPtr values[LAMBDA_MAX_ARG];
                unsigned int values_size = ListToArray(values, dcast_ex_Pair(args[i]));

                if(values_size != 2) {
                    sprintf(ex_buf, "malformed_cond");
                    throw_jump();
                }

                Symbol *s_val = dcast_Symbol(values[0]);

                Bool *b_val;

                if(s_val && s_val->id == ID_ELSE)
                    ret = eval(values[1], e);
                else if((b_val = dcast_Bool(eval(values[0],e))))
                    if(b_val->value) {
                        ret = eval(values[1], e);
                        goto end;
                    }
            }
            sprintf(ex_buf, "not_implemented");
            throw_jump();
        } else if(f && f->id == ID_QUOTE) {
            if(argc != 1) {
                sprintf(ex_buf, "malformed_quote");
                throw_jump();
            }

            ret = args[0];
        } else if(f && f->id == ID_DEFINE) {
            if(argc != 2) {
                sprintf(ex_buf,"not_implemented");
                throw_jump();
            }

            Symbol *symbol;

            if((symbol = dcast_Symbol(args[0]))) {
                env_define(e, symbol->id, eval(args[1],e));
                ret = ucast(symbol);
            } else {
                sprintf(ex_buf, "malformed_define");
                throw_jump();
            }
        } else if(f && f->id == ID_SET) {
            if(argc != 2) {
                sprintf(ex_buf, "malformed_set:args expected 2 but get %d" ,argc);
                throw_jump();
            }
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

                for(unsigned int i=0; i<argc; i++) {
                    eval_args[eval_argc++] = eval(args[i], e);
                }

                ret = func->func(eval_argc, eval_args);
            } else if((lambda = dcast_LambdaProcedure(callee))) {
                GCPtr eval_args[LAMBDA_MAX_ARG];
            
                for(unsigned int i=0; i<argc; i++)
                    eval_args[i] = eval(args[i], e);

                Environment *e_lambda = alloc_Environment(e);

                if(argc != (unsigned int)lambda->argc) {
                    sprintf(ex_buf, "not_implemented: wrong number of args");
                    throw_jump();
                }
                
                for(unsigned int i=0; i<argc; i++) {
                    env_define(e_lambda, lambda->argv[i], eval_args[i]);
                }
                
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
        //cerr << "<" << show(a) << " "<< show(ret) << endl << endl;
        return ret;
    }
    sprintf(ex_buf, "not_implemented:%d", a->tag);
    throw_jump();
}
