#pragma once
#include "exceptions.hpp"
#include "gc.hpp"
#include "gc_objects.hpp"
#include "generic_functions.hpp"

namespace sucheme{

   template<typename T>
   inline void ListForeach(Pair *list, T callback) {
        Pair *l = list;
        Pair *ll;

        for(;;){
            callback(l->car);
            ll = dcast<Pair>(l->cdr);
            if(!ll) break;
            l = ll;
        }

        if(!dcast<Empty>(l->cdr)) {
            sprintf(ex_buf, "improper_list");
            throw_jump();
        }
    }

    inline unsigned int list_length(Pair *list) {
        unsigned int ret = 0;
        ListForeach(list, [&](GCPtr ){ret++;});
        return ret;
    }

    inline unsigned int ListToArray(GCPtr a[],Pair *list) {
        unsigned int i=0;
        ListForeach(list,[&](GCPtr v){
                if(i >= LAMBDA_MAX_ARG) {
                    sprintf(ex_buf, "too_many_argument");
                    throw_jump();
                }
                a[i++] = v;
            });
        return i;
    }

    inline Pair* cons(GCPtr a, GCPtr l)
    {
        return alloc<Pair>(a, l);
    }

    inline GCPtr make_list() {
        return ucast(alloc<Empty>());
    }

    template<class... Rest>
    GCPtr make_list(GCPtr val, Rest... rest)
    {
        return ucast(cons(val, make_list(rest...)));
    }

    inline Empty *nil()
    {
        return alloc<Empty>();
    }

    inline Symbol *make_symbol(const char *name)
    {
        return alloc<Symbol>(name);
    }
}
