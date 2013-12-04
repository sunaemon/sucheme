#include <stdio.h>
#include "gc.h"
#include "exceptions.h"
#include "environment.h"
#include "show.h"

char *mem[2];
char *scaned;
char *unscaned;
int memory_in_used;

void init_gc()
{
    memory_in_used = 0;
    mem[0] = (char*)malloc(memsize);
    mem[1] = (char*)malloc(memsize);
    unscaned = scaned = mem[memory_in_used];
}

void term_gc()
{
    free(mem[0]);
    free(mem[1]);
}
    
int rpos_inactive_mem(void *ptr)
{
    return (char *)ptr - mem[1-memory_in_used];
}

int rpos_active_mem(void *ptr)
{
    return (char *)ptr - mem[memory_in_used];
}

inline bool have_to_copy(void *ptr)
{
    bool in_active_buf = 0 <= rpos_active_mem(ptr) && rpos_active_mem(ptr) <= memsize;
    bool in_inactive_buf = 0 <= rpos_inactive_mem(ptr) && rpos_inactive_mem(ptr) <= memsize;
    if(!in_active_buf && !in_inactive_buf) {
        sprintf(ex_buf, "object_not_under_gc_control: %d, %d", rpos_active_mem(ptr), rpos_inactive_mem(ptr));
        longjmp(ex_jbuf,0);
    }
    return in_inactive_buf;
}
    
GCPtr copy(GCPtr val) {
    Symbol *s;
    Number *n;
    Bool *b;
    Procedure *proc;
    LambdaProcedure *lambdaproc;
    Pair *p;
    Environment *e;
    EnvironmentMap *a;

    if(!have_to_copy(val))
        return val;
#ifdef GC_DEBUG
    cerr << "whereis:" << memory_location(val->whereis) << " " << memory_location(val) << endl;
#endif
    if(val->whereis != val)
        return val->whereis;

    if((b = dcast_Bool(val)))
        return val->whereis = ucast(alloc_Bool(b->value));
    else if((n = dcast_Number(val)))
        return val->whereis = ucast(alloc_Number(n->integer));
    else if((s = dcast_Symbol(val)))
        return val->whereis = ucast(alloc_Symbol(s->id));
    else if((p = dcast_Pair(val)))
        return val->whereis = ucast(alloc_Pair(p->car, p->cdr));
    else if(dcast_Empty(val))
        return val->whereis = ucast(alloc_Empty());
    else if((proc = dcast_Procedure(val)))
        return val->whereis = ucast(alloc_Procedure(proc->func));
    else if((lambdaproc = dcast_LambdaProcedure(val))) {
        LambdaProcedure *new_lp = alloc_LambdaProcedure(lambdaproc->body, lambdaproc->environment);
        new_lp -> argc = lambdaproc->argc;
        for(int i=0; i<lambdaproc->argc; i++)
            new_lp -> argv[i] = lambdaproc->argv[i];
        return val->whereis = ucast(new_lp);
    } else if((e = dcast_Environment(val))) {
        Environment *new_e =alloc_Environment(e->parent);
        new_e->env_map = e->env_map;
        return val->whereis = ucast(new_e);
    } else if((a = dcast_EnvironmentMap(val))) {
        EnvironmentMap *new_a = alloc_EnvironmentMap(a->id, a->val);
        new_a->g = a->g;
        new_a->l = a->l;
        return val->whereis = ucast(new_a);
    } else {
        throw_jumpf("not_implemented");
    }
}

unsigned long allocated_memory()
{
    return memsize - (unscaned -   mem[memory_in_used]);
}

   
void run_gc(Environment **e)
{
#ifdef GC_DEBUG
    unsigned long beforesize = unscaned - mem[memory_in_used];
#endif
    memory_in_used = 1-memory_in_used;
    scaned = unscaned = mem[memory_in_used];
    
    //fprintf(stderr, "rungc called\n");
    
    if(!*e) {
#ifdef GC_DEBUG
        fprintf(stderr, "run_gc called. memory usage: %ld -> 0\n", beforesize);
#endif
        return;
    }
        
    *e = (Environment*)copy(ucast(*e));
        
    while(scaned < unscaned) {
#ifdef GC_DEBUG
        fprintf(stderr, "%d\n", scaned - mem[0]);
        fprintf(stderr, "%d\n", unscaned - mem[0]);
#endif
            
        GCPtr val = ucast(scaned);

#ifdef GC_DEBUG
        fprintf(stderr, "\n%d\n", rpos_active_mem(val));
#endif
        //cerr << endl << rpos_active_mem(val)  << endl << showptr(val);
        Pair *p;
        Environment *e;
        EnvironmentMap *a;
        LambdaProcedure *lambdaproc;
        
        if(dcast_Bool(val))
            scaned += sizeof(Bool);
        else if(dcast_Number(val))
            scaned += sizeof(Number);
        else if(dcast_Symbol(val)) {
            scaned += sizeof(Symbol);
        } else if(dcast_Empty(val))
            scaned += sizeof(Empty);
        else if(dcast_Procedure(val))
            scaned += sizeof(Procedure);
        else if((p = dcast_Pair(val))) {
            p->car = copy(p->car);
            p->cdr = copy(p->cdr);
            scaned += sizeof(Pair);
        } else if((lambdaproc = dcast_LambdaProcedure(val))) {
            lambdaproc->body = copy(ucast(lambdaproc->body));
            lambdaproc->environment = (Environment*)copy(ucast(lambdaproc->environment));
            scaned += sizeof(LambdaProcedure);
        } else if((e = dcast_Environment(val))) {
            e->env_map = (EnvironmentMap*)copy(ucast(e->env_map));
            if(e->parent)
                e->parent = (Environment*)copy(ucast(e->parent));
            scaned += sizeof(Environment);
        } else if((a = dcast_EnvironmentMap(val))) {
            a->val = copy(a->val);
            if(a->g)
                a->g = (EnvironmentMap*)copy(ucast(a->g));
            if(a->l)
                a->l = (EnvironmentMap*)copy(ucast(a->l));
            scaned += sizeof(EnvironmentMap);
        }
        else {
            throw_jumpf("not_implemented");
        }
#ifdef GC_DEBUG        
        fprintf(stderr, "end %d\n", rpos_active_mem(val));
#endif
    }

#ifdef GC_DEBUG        
    unsigned long aftersize = unscaned - mem[memory_in_used];
        
    fprintf(stderr, "run_gc called, memory usage: %ld -> %ld\n", beforesize, aftersize);
#endif
}
