#pragma once
#include <iostream>
#include <typeinfo>
#include "exceptions.hpp"

namespace sucheme{
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

    using std::cerr;
    using std::endl;

    template<typename T, class ...Args>
    inline T *alloc(Args &&...args)
    {
        if((unsigned long)(memsize - (unscaned - mem[memory_in_used])) < sizeof(T))
            throw no_memory("no memory. please run gc");
        
        //fprintf(stderr, "alloc %40s (%2ld) from:%10d rest:%10ld\n", typeid(T).name(), sizeof(T), rpos_active_mem(unscaned), allocated_memory());
        
        T *ret = new(unscaned) T(std::forward<Args>(args)...);
        //T *ret = unscaned;
        //init(ret, std::forward<Args>(args)...);
        ret->whereis = ret; 
        unscaned+=sizeof(T);
        return ret;
    }
}

