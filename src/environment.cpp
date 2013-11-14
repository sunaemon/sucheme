#include "environment.hpp"

namespace sucheme
{
    using std::string;
    using std::ostringstream;
    using std::get;
    using std::move;
    using std::unique_ptr;
    using std::map;
    using std::cerr;
    using std::endl;

    struct LispVal;

    bool have(const Environment *e, const string &name)  {
        return e->env_map.find(name) != e->env_map.end();
    }

    shared_ptr<LispVal> lookup(const Environment *e, const string &name)  {
        auto i = e->env_map.find(name);
        
        if(i != e->env_map.end()) {
            return i->second;
        } else {
            if(e->parent)
                return lookup(e->parent.get(), name);
            else {
                //for(auto &i : env_map)
                //    cerr << i.first << endl;
                throw unbouded_variable("unbouded_variable:" + name);
            }
        }
    }
        
    void define(Environment *e, const string &name, const shared_ptr<LispVal> &value) {
        e->env_map[name] = value; //copy
    }

    void set(Environment *e, const string &name, const shared_ptr<LispVal> &value) {
        auto i = e->env_map.find(name);
        
        if(i != e->env_map.end()) {
            i->second = value; // copy
        } else {
            if(e->parent)
                set(e->parent.get(),name, value);
            else
                throw unbouded_variable("unbouded_variable:" + name);
        }
    }
}
