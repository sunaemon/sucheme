#include "parser.hpp"

namespace sucheme{

    std::string Number::show() {
        return std::to_string(this->integer);
    }

    std::string Symbol::show() {
        return this->name;
    }

    std::string List::show() {
        std::stringstream ost;
        ost << "(";
        bool flag = false;
        for(auto a : l) {
            if(!flag) flag=true;
            else ost << " ";
            ost << show(a);
        }
        ost << ")";
        return ost.str();
     }

}
