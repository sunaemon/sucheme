#pragma once
#include <string>
#include <memory>
#include <vector>
#include "lispval.hpp"
#include "exceptions.hpp"
#include "gc.hpp"
#include "generic_functions.hpp"

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::dynamic_pointer_cast;

   template<typename T>
   void ListForeach(Pair *list, T callback) {
        Pair *l = list;
        Pair *ll;

        for(;;){
            callback(l->car);
            ll = dcast<Pair>(l->cdr);
            if(!ll) break;
            l = ll;
        }

        if(typeid(*l->cdr) != typeid(Empty))
            throw improper_list();
    }

    vector<GCObject*> ListToVector(Pair* list);

    inline GCObject *make_list() {
        return alloc<Empty>();
    }

    inline Pair* cons(GCObject *a, GCObject *l)
    {
        return alloc<Pair>(a, l);
    }

    template<class... Rest>
    GCObject* make_list(GCObject *val, Rest... rest)
    {
        return cons(val, make_list(rest...));
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
