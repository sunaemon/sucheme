#pragma once
#include <string>
#include <memory>
#include <vector>
#include "lispval.hpp"
#include "exceptions.hpp"

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::dynamic_pointer_cast;

   template<typename T>
   void ListForeach(Pair *list, T callback) {
        Pair *l = list.get();
        Pair *ll;

        for(;;){
            callback(l->car);
            ll = dynamic_cast<Pair*>(l->cdr.get());
            if(!ll) break;
            l = ll;
        }

        if(typeid(*l->cdr.get()) != typeid(Empty))
            throw improper_list();
    }

    vector<LispVal*> ListToVector(const Pair* list);

    inline LispVal *make_list() {
        return alloc<Empty>();
    }

    inline Pair* cons(LispVal *a, ispVal *l)
    {
        return alloc<Pair>(a, l);
    }

    template<class... Rest>
    LispVal* make_list(const LispVal *val, const Rest&... rest)
    {
        return cons(val, make_list(rest...));
    }

    inline Empty *nil()
    {
        return alloc<Empty>();
    }


    inline Symbol> *make_symbol(const string &name)
    {
        return alloc<Symbol>(name);
    }
}
