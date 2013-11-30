#include "intern.hpp"
#include <string>
#include <map>
#include "exceptions.hpp"
#include <stdio.h>

namespace sucheme{
    using std::string;
    using std::map;

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

    const char *extern_symbol(int id)
    {
        for(auto &p : symbol_map)
            if(p.second == id) {
                //cerr << "extern : recall   " << p.first << " as " <<  id << endl;        
                return p.first.c_str();
            }
        sprintf(ex_buf, "not_interned_id:%d", id);
        longjmp(ex_jbuf,0);
    }
}
