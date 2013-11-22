#include "intern.hpp"
#include <string>
#include <map>
#include "exceptions.hpp"
#include <sstream>
#include <iostream>
namespace sucheme{
    using std::string;
    using std::map;
    using std::stringstream;
    using std::cerr;
    using std::endl;

    static map<string, int> symbol_map { {"lambda", ID_LAMBDA}, {"cond",ID_COND}, {"quote", ID_QUOTE}, {"define", ID_DEFINE}, {"define-macro", ID_DEFINE_MACRO}, {"set!",ID_SET}, {"begin", ID_BEGIN},{"else", ID_ELSE} };
    static int next_id = REGISTERED_ID_COUNT;

    int intern_symbol(const char *name)
    {
        auto itr = symbol_map.find(name);

        if(itr == symbol_map.end()) {
            symbol_map[name] = next_id;
            //cerr << "intern : register " << name << " as " <<  next_id << endl;
            return next_id++;
        }

        //cerr << "intern : recall   " << name << " as " <<  itr->second << endl;
        return itr->second;
    }

    string extern_symbol(int id)
    {
        for(auto &p : symbol_map)
            if(p.second == id) {
                //cerr << "extern : recall   " << p.first << " as " <<  id << endl;        
                return p.first;
            }
        stringstream ost;

        ost << "not_interned_id : " << id;
        
        throw not_interned_symbol(ost.str());
    }
}
