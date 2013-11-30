#pragma once
#include <string>
#include <memory>
#include <vector>
#include "exceptions.hpp"
#include "gc.hpp"
#include "gc_objects.hpp"
#include "generic_functions.hpp"

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::dynamic_pointer_cast;

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

        if(!dcast<Empty>(l->cdr))
            throw improper_list();
    }

    inline unsigned int list_length(Pair *list) {
        unsigned int ret = 0;
        ListForeach(list, [&](GCPtr ){ret++;});
        return ret;
    }

    inline vector_ptr ListToVector(Pair *list) {
        vector_ptr ret;
        ret.reserve(10);
        ListForeach(list,[&ret](GCPtr v){ret.push_back(v);});
        return ret;
    }

    inline GCPtr make_list() {
        return ucast(alloc<Empty>());
    }

    inline Pair* cons(GCPtr a, GCPtr l)
    {
        return alloc<Pair>(a, l);
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

    inline Symbol *make_symbol(const string &name)
    {
        return alloc<Symbol>(name);
    }
}
