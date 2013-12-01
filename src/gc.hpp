#pragma once
#include "exceptions.hpp"
#include "gc_objects.hpp"

const int memsize = 100000000;

extern char *mem[2];
extern char *scaned;
extern char *unscaned;
extern int memory_in_used;

struct Environment;

void run_gc(Environment *&e);
void init_gc();
void term_gc();

int rpos_inactive_mem(void *ptr);
int rpos_active_mem(void *ptr);

unsigned long allocated_memory();

#define alloc_spec(T, a, p)                                             \
    inline T* alloc_##T p                                              \
    {                                                                   \
        if((unsigned long)(memsize - (unscaned - mem[memory_in_used])) < sizeof(T)) { \
            sprintf(ex_buf, "no memory. please run gc");                \
            throw_jump();                                               \
        }                                                               \
        T *ret = (T*)unscaned;                                      \
        init_##T a;                                               \
        ret->obj.whereis = ucast(ret);                                  \
        unscaned+=sizeof(T);                                            \
        return ret;                                                     \
    }                                                                       

alloc_spec(EnvironmentMap, (ret,id,val), (int id, GCPtr val))
alloc_spec(Environment, (ret,parent), (Environment *parent))
alloc_spec(Number, (ret,integer), (int integer))
alloc_spec(Bool, (ret,var), (bool var))
alloc_spec(Symbol, (ret,id), (int id))
alloc_spec(Empty, (ret),())
alloc_spec(Procedure, (ret, func),(const subr func))
alloc_spec(Pair, (ret, car, cdr), (GCPtr car, GCPtr cdr))
alloc_spec(LambdaProcedure, (ret, body, e), (GCPtr body, Environment *e))


