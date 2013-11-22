#include <utility>
#include <stdio.h>
#include "gc.hpp"
#include "exceptions.hpp"
#include "generic_functions.hpp"
#include "environment.hpp"

namespace sucheme{
    using std::to_string;

    char *mem[2];
    char *scaned;
    char *unscaned;
    int memory_in_used;

    void init_gc()
    {
        memory_in_used = 0;
        mem[0] = (char*)malloc(100000000);
        mem[1] = (char*)malloc(100000000);
        unscaned = scaned = mem[memory_in_used];
    }

    void term_gc()
    {
        free(mem[0]);
        free(mem[1]);
    }

    void copy(GCObject *&val) {
        if(auto b = dcast<Bool>(val))
            val = alloc<Bool>(*b);
        else if(auto n = dcast<Number>(val))
            val = alloc<Number>(*n);
        else if(auto s = dcast<Symbol>(val))
            val = alloc<Symbol>(*s);
        else if(auto p = dcast<Pair>(val))
            val = alloc<Pair>(*p);
        else if(auto em = dcast<Empty>(val))
            val = alloc<Empty>(*em);
        else if(auto pc = dcast<Procedure>(val))
            val = alloc<Procedure>(*pc);
        else if(auto lp = dcast<LambdaProcedure>(val))
            val = alloc<LambdaProcedure>(*lp);
        else if(auto lm = dcast<LambdaMacro>(val))
            val = alloc<LambdaMacro>(*lm);
        else if(auto e = dcast<Environment>(val))
            val = alloc<Environment>(*e);
        else if(auto a = dcast<EnvironmentMap>(val))
            val = alloc<EnvironmentMap>(*a);
        else 
            throw not_implemented();
    }
    
    int rpos_inactive_mem(void *ptr)
    {
        return (char *)ptr - mem[1-memory_in_used];
    }

    int rpos_active_mem(void *ptr)
    {
        return (char *)ptr - mem[memory_in_used];
    }

    inline bool have_to_move(void *ptr)
    {
        auto in_active_buf = 0 <= rpos_active_mem(ptr) && rpos_active_mem(ptr) <= memsize;
        auto in_inactive_buf = 0 <= rpos_inactive_mem(ptr) && rpos_inactive_mem(ptr) <= memsize;
        if(!in_active_buf && !in_inactive_buf)
            throw object_not_under_gc_control("object_not_under_gc_control" + to_string(rpos_active_mem(ptr)) + ", " + to_string(rpos_inactive_mem(ptr)));
        return in_inactive_buf;
    }

    unsigned long allocated_memory()
    {
        return memsize - (unscaned -   mem[memory_in_used]);
    }

   
    void run_gc(Environment *&e)
    {
        unsigned long beforesize = unscaned - mem[memory_in_used];
        memory_in_used = 1-memory_in_used;
        scaned = unscaned = mem[memory_in_used];
        
        if(!e) {
            fprintf(stderr, "run_gc called memory usage: %ld -> 0\n", beforesize);
            return;
        }
        
        e = alloc<Environment>(*e);
        
        while(scaned < unscaned) {
            //cerr << scaned - mem[0] << endl;
            //cerr << unscaned - mem[0] << endl;
            
            GCObject *val = reinterpret_cast<GCObject*>(scaned);

            cerr << endl << rpos_active_mem(val)  << endl << showptr(val);
        
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
                copy(p->car);
                copy(p->cdr);
                scaned += sizeof(Pair);
            } else if(auto lp = dcast<LambdaProcedure>(val)) {
                if(have_to_move(lp->body))
                    lp->body = alloc<Pair>(*lp->body);
                if(have_to_move(lp->environment))
                    lp->environment = alloc<Environment>(*lp->environment);
                scaned += sizeof(LambdaProcedure);
            } else if(auto lm = dcast<LambdaMacro>(val)) {
                if(have_to_move(lm->body))
                    lm->body = alloc<Pair>(*lm->body);
                if(have_to_move(lm->environment))
                    lm->environment = alloc<Environment>(*lm->environment);
                scaned += sizeof(LambdaMacro);
            } else if(auto e = dcast<Environment>(val)) {
                if(have_to_move(e->env_map))
                    e->env_map = alloc<EnvironmentMap>(*e->env_map);
                if(e->parent)
                    if(have_to_move(e->parent))
                        e->parent = alloc<Environment>(*e->parent);
                scaned += sizeof(Environment);
            } else if(auto a = dcast<EnvironmentMap>(val)) {
                if(have_to_move(a->val))
                    copy(a->val);
                if(a->g)
                    if(have_to_move(a->g))
                       a->g = alloc<EnvironmentMap>(*a->g);
                if(a->l)
                    if(have_to_move(a->l))
                        a->l = alloc<EnvironmentMap>(*a->l);
                scaned += sizeof(EnvironmentMap);
            }
           else 
                throw not_implemented();

           cerr << "end " << rpos_active_mem(val) << endl;
        }

         unsigned long aftersize = unscaned - mem[memory_in_used];
        
         fprintf(stderr, "run_gc called memory usage: %ld -> %ld\n", beforesize, aftersize);
    }
}
