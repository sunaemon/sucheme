#include "parser.hpp"
#include <sstream>
#include <iostream>

namespace sucheme{

    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;

    string LispVal::show() const {
        throw std::exception();
    }

    string Number::show() const  {
        return to_string(this->integer);
    }

    string Symbol::show() const {
        return this->name;
    }

    string Pair::show() const {
        vector<string> buf;
        
        auto *next=this;

        for(;;){
            buf.push_back(next->car->show());

            if(typeid(*next->cdr.get()) != typeid(Pair))
                break;

            next = static_cast<Pair*>(next->cdr.get());
        }

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
    
    string Procedure::show() const {
        stringstream ost;
        ost << "<Procedure 0x" << std::hex << (unsigned long)func << ">";
        return ost.str();
    }

}
