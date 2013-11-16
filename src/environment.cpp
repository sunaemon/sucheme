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

    struct env_map_find_return
    {
        bool found;
        shared_ptr<LispVal> val;
    };

    env_map_find_return env_map_find(const Environment_Map *a, string name)
    {
        const Environment_Map *next = a;
        for(;;) {
            if(name < next->name) {
                if(next->l)
                    next=next->l.get();
                else
                    return {false, NULL};
            } else if(name > next->name) {
                if(next->g)
                    next=next->g.get();
                else
                    return {false, NULL};
            } else if(name == next->name){
                return {true, next->val};
            }
        }
    }

    bool env_map_set(Environment_Map *a, string name, const shared_ptr<LispVal> &val)
    {
        Environment_Map *next = a;
        for(;;) {
            if(name < next->name) {
                if(next->l)
                    next=next->l.get();
                else
                    return false;
            } else if(name > next->name) {
                if(next->g)
                    next=next->g.get();
                else
                    return false;
            } else if(name == next->name){
                next->val = val;
                return true;
            }
        }
    }

    void env_map_add(Environment_Map *a, string name, shared_ptr<LispVal> val)
    {
        Environment_Map *next = a;
        for(;;) {
            if(name < next->name) {
                if(next->l)
                    next=next->l.get();
                else {
                    next->l = make_shared<Environment_Map>();
                    next->l->name = name;
                    next->l->val = val;
                    return;
                }
            } else if(name > next->name) {
                if(next->g)
                    next=next->g.get();
                else {
                    next->g = make_shared<Environment_Map>();
                    next->g->name = name;
                    next->g->val = val;
                    return;
                }
            } else if(name == next->name){
                if(val)
                    next->val = val;
                return;
            }
        }
    }

    bool env_have(const Environment *e, const string &name)  {
        auto ret = env_map_find(&e->env_map, name);
        return ret.found;
    }


    shared_ptr<LispVal> env_lookup(const Environment *e, const string &name)  {
        shared_ptr<LispVal> val;
        auto ret = env_map_find(&e->env_map, name);
        if(ret.found)
            return ret.val;
        else {
            if(e->parent)
                return env_lookup(e->parent.get(), name);
            else
                throw unbouded_variable("unbouded_variable:" + name);
        }
    }

    void env_define(Environment *e, const string &name, const shared_ptr<LispVal> &value) {
        env_map_add(&e->env_map, name, value); // copy
    }

    void env_set(Environment *e, const string &name, const shared_ptr<LispVal> &value) {
        shared_ptr<LispVal> val;
        if(!env_map_set(&e->env_map, name, value)){
            if(e->parent) {
                if(!env_map_set(&e->env_map, name, value))
                    throw unbouded_variable("unbouded_variable:" + name);    
            } else
                throw unbouded_variable("unbouded_variable:" + name);
        }
    }
}
