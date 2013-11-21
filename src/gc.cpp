#include <utility>
#include <stdio.h>
#include "gc.hpp"
#include "exceptions.hpp"

namespace sucheme{
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

    template<class T0, class T1>
    T0 *dcast(T1 *val) {
        return dynamic_cast<T0*>(val);
    }

    void copy(LispVal *&val) {
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
        else if(auto a = dcast<Environment_Map>(val))
            val = alloc<Environment_Map>(*a);
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
            //fprintf(stderr, "run_gc called memory usage: %ld -> 0\n", beforesize);
            return;
        }

        e = alloc<Environment>(*e);

        while(scaned < unscaned) {
            LispVal *val = reinterpret_cast<LispVal*>(scaned);

            if(dcast<Bool>(val))
                scaned += sizeof(Bool);
            else if(dcast<Number>(val))
                scaned += sizeof(Number);
            else if(dcast<Symbol>(val))
                scaned += sizeof(Symbol);
            else if(dcast<Empty>(val))
                scaned += sizeof(Empty);
            else if(dcast<Procedure>(val))
                scaned += sizeof(Procedure);
            else if(auto p = dcast<Pair>(val)) {
                copy(p->car);
                copy(p->cdr);
                scaned += sizeof(Pair);
            } else if(auto lp = dcast<LambdaProcedure>(val)) {
                lp->body = alloc<Pair>(*lp->body);
                lp->environment = alloc<Environment>(*lp->environment);
                scaned += sizeof(LambdaProcedure);
            } else if(auto lm = dcast<LambdaMacro>(val)) {
                lm->body = alloc<Pair>(*lm->body);
                lm->environment = alloc<Environment>(*lm->environment);
                scaned += sizeof(LambdaMacro);
            } else if(auto e = dcast<Environment>(val)) {
                e->env_map = alloc<Environment_Map>(*e->env_map);
                if(e->parent)
                    e->parent = alloc<Environment>(*e->parent);
                scaned += sizeof(Environment);
            } else if(auto a = dcast<Environment_Map>(val)) {
                if(a->g)
                    a->g = alloc<Environment_Map>(*a->g);
                if(a->l)
                    a->l = alloc<Environment_Map>(*a->l);
                copy(a->val);
                scaned += sizeof(Environment_Map);
            }
            else 
                throw not_implemented();
        }

        // unsigned long aftersize = unscaned - mem[memory_in_used];
        
         //fprintf(stderr, "run_gc called memory usage: %ld -> %ld\n", beforesize, aftersize);
    }
}
