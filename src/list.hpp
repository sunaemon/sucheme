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
    void ListForeach(const shared_ptr<Pair> &list, T callback) {
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

    vector<shared_ptr<LispVal> > ListToVector(const shared_ptr<Pair> &list);

    inline shared_ptr<LispVal> make_list() {
        return make_shared<Empty>();
    }

    template<class... Rest>
    shared_ptr<LispVal> make_list(const shared_ptr<LispVal> &val, const Rest&... rest)
    {
        return make_shared<Pair>(val, make_list(rest...));
    }

    inline shared_ptr<Empty> nil()
    {
        return make_shared<Empty>();
    }

    inline shared_ptr<Pair> cons(const shared_ptr<LispVal> &a, const shared_ptr<LispVal> &l)
    {
        return make_shared<Pair>(a, l);
    }

    inline shared_ptr<Symbol> make_symbol(const string &name)
    {
        return make_shared<Symbol>(name);
    }
}
