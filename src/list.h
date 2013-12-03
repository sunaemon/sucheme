#pragma once
#include "exceptions.h"
#include "gc.h"
#include "gc_objects.h"

/*template<typename T>
inline void ListForeach(Pair *list, T callback) {
    Pair *l = list;
    Pair *ll;

    for(;;){
        callback(l->car);
        ll = dcast_Pair(l->cdr);
        if(!ll) break;
        l = ll;
    }

    if(!dcast_Empty(l->cdr)) {
        sprintf(ex_buf, "improper_list");
        throw_jump();
    }
    }*/

inline unsigned int ListToArray(GCPtr a[],Pair *list) {
    unsigned int i=0;

    Pair *l = list;
    Pair *ll;

    for(;;){

        if(i >= LAMBDA_MAX_ARG) {
            sprintf(ex_buf, "too_many_argument");
            throw_jump();
        }
        a[i++] = l->car;

        ll = dcast_Pair(l->cdr);
        if(!ll) break;
        l = ll;
    }

    if(!dcast_Empty(l->cdr)) {
        sprintf(ex_buf, "improper_list");
        throw_jump();
    }

    return i;
}

inline Pair* cons(GCPtr a, GCPtr l)
{
    return alloc_Pair(a, l);
}

inline Empty *nil()
{
    return alloc_Empty();
}

inline Symbol *make_symbol(const char *name)
{
    return alloc_Symbol(intern_symbol(name));
}
