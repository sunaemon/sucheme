#include "parser.hpp"
#include <sstream>
#include <iostream>

namespace sucheme{

    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;

    string LispVal::show() {
        return "sleislfij";
    }

    string Number::show() {
        return to_string(this->integer);
    }

    string Symbol::show() {
        return this->name;
    }

    string Pair::show() {
        vector<string> buf;
        
        auto *next=this;

        do{
            buf.push_back(next->car->show());
            if(typeid(*next->cdr.get()) != typeid(Pair)) {
                break;
            }
            next = static_cast<Pair*>(next->cdr.get());
        }while(true);

        if(typeid(*next->cdr.get()) == typeid(Empty)) {
            stringstream ost;
            ost << "(";
            bool flag = false;
            for(auto &s : buf) {
                if(flag)
                    ost << " ";
                else
                    flag = true;
                ost << s;
            }
            ost << ")";
            return ost.str();
        } else {
            throw std::exception();
        }
     }

}
