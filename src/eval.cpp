#include <sstream>
#include "parser.hpp"
#include <map>
#include <vector>

/*std::string show(const LispVal &val)
{
    return nv::match<std::string> (
        val,
        nv::case_<NumberData>([](number n) {
                return std::to_string(n.integer);
            }),
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
        nv::case_<SymbolData>([](const std::string &s) {
                return s;
            }),
        );
        }*/

/*LispVal plus(const std::vector<LispVal> &val)
{
    int ret = 0;
    for(auto &a : val) {
        assert(nv::type(a) == typeid(NumberData));
        ret += nv::get<NumberData>(a).integer;
    }
    return 0;
    }*/

/*std::map<std::string, LispVal> value_map = {;

LispVal eval(const LispVal &val)
{
    return nv::match<LispVal>(val,
                              nv::case_<NumberData>([](number n){return init<NumberData>(n)}),
                              nv::case_<ListData>([](std::list<LispVal> l) {
                                      std::vector<LispVal> vl;
                                      if(l.size() == 0)
                                          return return init<ListData>(l);

                                      for(auto i = list.begin()+1; i < list.end(); i++) {
                                          vl.push_back(eval(*i));
                                      }
                                      
                                      
                                  }),
                              nv::case_<SymbolData>([](const std::string &s){
                                      return value_map[s];
                                  })
                              nv::case_<StandardProcedure>([](const standard_procedure &f){
                                      return init<StandardProcedure>(f);
                                  })
        );
        }*/
