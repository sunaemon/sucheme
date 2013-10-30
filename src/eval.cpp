#include <sstream>
#include "parser.hpp"
#include <map>
#include <vector>
#include <iostream>

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::unique_ptr;

    unique_ptr<LispVal> add(const vector<unique_ptr<LispVal> > &arg) {
        int ret=0;
        for(auto &i : arg) {
            auto a = dynamic_cast<Number*>(i.get());

            if(nullptr == a)
                throw std::exception();

            ret += a->integer;
        }
        return make_unique<Number>(ret);
    }

    std::map<std::string, unique_ptr<LispVal> > value_map;

    unique_ptr<LispVal> Symbol::eval() {
        if(name == "+")
            return make_unique<Procedure>(add);
        return value_map[name]->clone();
    }

    unique_ptr<LispVal> Pair::eval() {
            
        vector<unique_ptr<LispVal> > args;

        auto next = this;

        for(;;){
            if(typeid(*next->cdr.get()) != typeid(Pair))
                break;

            next = static_cast<Pair*>(next->cdr.get());

            args.push_back(next->car->eval());
        }

        if(typeid(*next->cdr.get()) != typeid(Empty))
            throw 1;

        unique_ptr<Procedure> func(dynamic_cast<Procedure *>(car->eval().release()));

        return func ? move(func->func(args)) : throw "test";
    }
}
