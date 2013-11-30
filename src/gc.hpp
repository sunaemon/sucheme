#pragma once
#include "exceptions.hpp"
#include "gc_objects.hpp"
#include <new>


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

template<typename T, class ...Args>
inline T *alloc(Args &&...args)
{
    if((unsigned long)(memsize - (unscaned - mem[memory_in_used])) < sizeof(T)) {
        sprintf(ex_buf, "no memory. please run gc");
        throw_jump();
    }
        
        
    T *ret = new(unscaned) T(args...);
    ret->obj.whereis = ucast(ret);
    unscaned+=sizeof(T);
    return ret;
}
