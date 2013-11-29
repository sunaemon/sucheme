#include <stdio.h>
#include "gc.hpp"
#include "exceptions.hpp"
#include "generic_functions.hpp"
#include "environment.hpp"
#include "show.hpp"

namespace sucheme{
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
        auto in_active_buf = 0 <= rpos_active_mem(ptr) && rpos_active_mem(ptr) <= memsize;
        auto in_inactive_buf = 0 <= rpos_inactive_mem(ptr) && rpos_inactive_mem(ptr) <= memsize;
        if(!in_active_buf && !in_inactive_buf) {
            sprintf(ex_buf, "object_not_under_gc_control: %d, %d", rpos_active_mem(ptr), rpos_inactive_mem(ptr));
            throw object_not_under_gc_control(ex_buf);
        }
        return in_inactive_buf;
    }
    
    GCObject *copy(GCObject *val) {
        if(!have_to_copy(val))
            return val;

        //cerr << "whereis:" << memory_location(val->whereis) << " " << memory_location(val) << endl;

        if(val->whereis != val)
            return val->whereis;

        if(auto b = dcast<Bool>(val)) {
            return val->whereis = ucast(alloc<Bool>(*b));
        } else if(auto n = dcast<Number>(val))
            return val->whereis = ucast(alloc<Number>(*n));
        else if(auto s = dcast<Symbol>(val))
            return val->whereis = ucast(alloc<Symbol>(*s));
        else if(auto p = dcast<Pair>(val))
            return val->whereis = ucast(alloc<Pair>(*p));
        else if(auto em = dcast<Empty>(val))
            return val->whereis = ucast(alloc<Empty>(*em));
        else if(auto pc = dcast<Procedure>(val))
            return val->whereis = ucast(alloc<Procedure>(*pc));
        else if(auto lp = dcast<LambdaProcedure>(val))
            return val->whereis = ucast(alloc<LambdaProcedure>(*lp));
        else if(auto lm = dcast<LambdaMacro>(val))
            return val->whereis = ucast(alloc<LambdaMacro>(*lm));
        else if(auto e = dcast<Environment>(val))
            return val->whereis = ucast(alloc<Environment>(*e));
        else if(auto a = dcast<EnvironmentMap>(val))
            return val->whereis = ucast(alloc<EnvironmentMap>(*a));
        else 
            throw not_implemented();
    }
            


    unsigned long allocated_memory()
    {
        return memsize - (unscaned -   mem[memory_in_used]);
    }

   
    void run_gc(Environment *&e)
    {
        //unsigned long beforesize = unscaned - mem[memory_in_used];
        memory_in_used = 1-memory_in_used;
        scaned = unscaned = mem[memory_in_used];
        
        if(!e) {
            //fprintf(stderr, "run_gc called. memory usage: %ld -> 0\n", beforesize);
            return;
        }
        
        e = (Environment*)copy(ucast(e));
        
        while(scaned < unscaned) {
            //cerr << scaned - mem[0] << endl;
            //cerr << unscaned - mem[0] << endl;
            
            GCObject *val = reinterpret_cast<GCObject*>(scaned);

            //cerr << endl << rpos_active_mem(val)  << endl << showptr(val);
        
           if(dcast<Bool>(val))
                scaned += sizeof(Bool);
            else if(dcast<Number>(val))
                scaned += sizeof(Number);
            else if(dcast<Symbol>(val)) {
                scaned += sizeof(Symbol);
            } else if(dcast<Empty>(val))
                scaned += sizeof(Empty);
            else if(dcast<Procedure>(val))
                scaned += sizeof(Procedure);
            else if(auto p = dcast<Pair>(val)) {
                p->car = copy(p->car);
                p->cdr = copy(p->cdr);
                scaned += sizeof(Pair);
            } else if(auto lp = dcast<LambdaProcedure>(val)) {
                lp->body = (Pair*)copy(ucast(lp->body));
                lp->environment = (Environment*)copy(ucast(lp->environment));
                scaned += sizeof(LambdaProcedure);
            } else if(auto lm = dcast<LambdaMacro>(val)) {
                lm->body = (Pair*)copy(ucast(lm->body));
                lm->environment = (Environment*)copy(ucast(lm->environment));
                scaned += sizeof(LambdaMacro);
            } else if(auto e = dcast<Environment>(val)) {
                e->env_map = (EnvironmentMap*)copy(ucast(e->env_map));
                if(e->parent)
                    e->parent = (Environment*)copy(ucast(e->parent));
                scaned += sizeof(Environment);
            } else if(auto a = dcast<EnvironmentMap>(val)) {
                a->val = copy(a->val);
                if(a->g)
                    a->g = (EnvironmentMap*)copy(ucast(a->g));
                if(a->l)
                    a->l = (EnvironmentMap*)copy(ucast(a->l));
                scaned += sizeof(EnvironmentMap);
            }
           else 
                throw not_implemented();

           //cerr << "end " << rpos_active_mem(val) << endl;
        }

        //unsigned long aftersize = unscaned - mem[memory_in_used];
        
        //fprintf(stderr, "run_gc called, memory usage: %ld -> %ld\n", beforesize, aftersize);
    }
}
