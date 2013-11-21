#pragma once
#include <string>
#include <memory>
#include <vector>
#include "lispval.hpp"
#include "exceptions.hpp"
#include "gc.hpp"

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
            ll = dynamic_cast<Pair*>(l->cdr);
            if(!ll) break;
            l = ll;
        }

        if(typeid(*l->cdr) != typeid(Empty))
            throw improper_list();
    }

    vector<LispVal*> ListToVector(Pair* list);

    inline LispVal *make_list() {
        return alloc<Empty>();
    }

    inline Pair* cons(LispVal *a, LispVal *l)
    {
        return alloc<Pair>(a, l);
    }

    template<class... Rest>
    LispVal* make_list(LispVal *val, Rest... rest)
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
