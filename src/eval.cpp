#include <sstream>
#include "parser.hpp"
#include "environment.hpp"
#include <map>
#include <vector>
#include <iostream>

namespace sucheme{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::unique_ptr;
    using std::dynamic_pointer_cast;
 
    shared_ptr<LispVal> Symbol::eval(const Environment &e) {
        return e.lookup(name);
    }

    shared_ptr<LispVal> Pair::eval(const Environment &e) {
//        auto f = dynamic_pointer_cast<Symbol>(car);

        vector<shared_ptr<LispVal> > args;

        auto next = shared_from_this();

        for(;;){
            auto pcar = dynamic_pointer_cast<Pair>(next->cdr);

            if(!pcar) break;

            next = pcar;

            args.push_back(next->car->eval(e));
        }

        if(typeid(*next->cdr.get()) != typeid(Empty))
            throw 1;

        auto val = car->eval(e);

        auto func = dynamic_pointer_cast<Procedure>(val);
        
        return func ? func->func(args) : throw "test";
    }
}
