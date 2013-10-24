#include <sstream>
#include "parser.hpp"
#include <map>

std::string show(const LispVal &val)
{
    return nv::match<std::string> (val,
                                   nv::case_<NumberData>([](number n){return std::to_string(n.integer);}),
                                   nv::case_<ListData>([](std::list<LispVal> l) {
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
                                       }),
                                   nv::case_<SymbolData>([](const std::string &s){return s;})
        );
}

LispVal eval(const LispVal &val)
{
    return nv::match<LispVal>(val,
                              nv::case_<NumberData>([](number n){return init<NumberData>(n)}),
                              nv::case_<ListData>([](std::list<LispVal> l) {
                                      if(l.size() > 2) {
                                          (
                                      }
                                      
                                  }),
                              nv::case_<SymbolData>([](const std::string &s){throw exception();})
        );
}
