#pragma once
#include <iostream>
#include "environment.hpp"
namespace sucheme{
    const int memsize = 100000000;

    extern char *mem[2];
    extern char *scaned;
    extern char *unscaned;
    extern int memory_in_used;

    void run_gc(Environment *&e);
    void init_gc();
    void term_gc();

    unsigned long allocated_memory();

    using std::cerr;
    using std::endl;

    template<typename T, class ...Args>
    __attribute__((always_inline)) inline T *alloc(Args &&...args)
    {
        //if((unsigned long)(memsize - (scaned - mem[memory_in_used])) < sizeof(T))
            //run_gc();

        //fprintf(stderr, "alloc %40s (%2d) rest:%d\n", typeid(T).name(), sizeof(T), allocated_memory());
        
        T *ret = new(unscaned) T(std::forward<Args>(args)...);
        unscaned+=sizeof(T);
        return ret;
    }
}
