#include "list.hpp"
#include "parser.hpp"
#include "environment.hpp"
#include "show.hpp"
#include "exceptions.hpp"
#include "generic_functions.hpp"
#include "intern.hpp"
#include "gc_objects.hpp"

GCPtr eval(GCPtr a, Environment *e) {
    if(auto s = dcast<Symbol>(a))
        return env_lookup(e, s->id);
    if(auto n = dcast<Number>(a))
        return ucast(n);
    if(auto b = dcast<Bool>(a))
        return ucast(b);
    if(auto empty = dcast<Empty>(a))
        return ucast(empty);
    if(auto proc = dcast<Procedure>(a))
        return ucast(proc);
    if(auto lambdaproc = dcast<LambdaProcedure>(a))
        return ucast(lambdaproc);
    if(auto p = dcast<Pair>(a)) {
        GCPtr args[LAMBDA_MAX_ARG];
        unsigned int argc = ListToArray(args, dcast_ex<Pair>(p->cdr));

        GCPtr ret = nullptr;
        
        //cerr << ">" << show(a) << endl << endl;

        auto f = dcast<Symbol>(p->car);
        if(f && f->id == ID_LAMBDA) {
            if(argc != 2) {
                sprintf(ex_buf, "malformed_lambda argsize: expected 2 get %d", argc);
                longjmp(ex_jbuf,0);
            }

            auto body = args[1];
            LambdaProcedure *l = alloc_LambdaProcedure(body, e);
            int i=0;
            ListForeach(dcast_ex<Pair>(args[0]),
                        [&](GCPtr v){
                            if(i<LAMBDA_MAX_ARG)
                                l->argv[i++] = dcast_ex<Symbol>(v)->id;
                            else {
                                sprintf(ex_buf, "too_many_argument");
                                throw_jump();
                            }
                        });
            l->argc = i;
            ret = ucast(l);
        } else if(f && f->id == ID_COND) {
            for(auto &i : args) {
                GCPtr values[LAMBDA_MAX_ARG];
                unsigned int values_size = ListToArray(values, dcast_ex<Pair>(i));

                if(values_size != 2) {
                    sprintf(ex_buf, "malformed_cond");
                    throw_jump();
                }

                auto val = dcast<Symbol>(values[0]);

                if(val && val->id == ID_ELSE)
                    ret = eval(values[1], e);
                else if(auto val = dcast<Bool>(eval(values[0],e)))
                    if(val->value) {
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

            if(auto symbol = dcast<Symbol>(args[0])) {
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
            env_set(e, dcast_ex<Symbol>(args[0])->id, eval(args[1], e));
            ret = ucast(nil());
        } else if(f && f->id == ID_BEGIN) {
            for(unsigned int i=0; i<argc; i++)
                ret = eval(args[i], e);
        } else { // application is function call
            auto callee = eval(p->car, e);
            
            if(auto func = dcast<Procedure>(callee)) {
                GCPtr eval_args[LAMBDA_MAX_ARG];

                int eval_argc=0;

                for(unsigned int i=0; i<argc; i++) {
                    eval_args[eval_argc++] = eval(args[i], e);
                }

                ret = func->call(eval_argc, eval_args);
            } else if(auto lambda = dcast<LambdaProcedure>(callee)) {
                GCPtr eval_args[LAMBDA_MAX_ARG];
            
                for(unsigned int i=0; i<argc; i++)
                    eval_args[i] = eval(args[i], e);

                auto e_lambda = alloc_Environment(e);

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
