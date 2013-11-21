#include "environment.hpp"
#include "gc.hpp"

namespace sucheme
{
    using std::string;
    using std::ostringstream;
    using std::cerr;
    using std::endl;

    struct LispVal;

    struct env_map_find_return
    {
        bool found;
        LispVal *val;
    };

    inline env_map_find_return env_map_find(const Environment_Map *a, string name)
    {
        const Environment_Map *next = a;
        for(;;) {
            if(name < next->name) {
                if(next->l)
                    next=next->l;
                else
                    return {false, NULL};
            } else if(name > next->name) {
                if(next->g)
                    next=next->g;
                else
                    return {false, NULL};
            } else if(name == next->name){
                return {true, next->val};
            }
        }
    }

    bool env_map_set(Environment_Map *a, string name, LispVal *val)
    {
        Environment_Map *next = a;
        for(;;) {
            if(name < next->name) {
                if(next->l)
                    next=next->l;
                else
                    return false;
            } else if(name > next->name) {
                if(next->g)
                    next=next->g;
                else
                    return false;
            } else if(name == next->name){
                next->val = val;
                return true;
            }
        }
    }

    void env_map_add(Environment_Map *a, string name, LispVal *val)
    {
        Environment_Map *next = a;
        for(;;) {
            if(name < next->name) {
                if(next->l)
                    next=next->l;
                else {
                    next->l = alloc<Environment_Map>(name,val);
                    return;
                }
            } else if(name > next->name) {
                if(next->g)
                    next=next->g;
                else {
                    next->g = alloc<Environment_Map>(name,val);
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
        if(e->env_map)
            return env_map_find(e->env_map, name).found;
        else
            return false;
    }

    LispVal *env_lookup(const Environment *e, const string &name)  {
        if(e->env_map) {
            auto ret = env_map_find(e->env_map, name);
            if(ret.found)
                return ret.val;
        }
        
        if(e->parent)
            return env_lookup(e->parent, name);
        else
            throw unbouded_variable("unbouded_variable:" + name);
    }

    void env_define(Environment *e, const string &name, LispVal *value) {
        if(e->env_map)
            env_map_add(e->env_map, name, value); // copy
        else {
            e->env_map = alloc<Environment_Map>(name,value);
        }
    }

    void env_set(Environment *e, const string &name, LispVal *value) {
        if(e->env_map) {
            if(!env_map_set(e->env_map, name, value)){
                if(e->parent) {
                    if(!env_map_set(e->env_map, name, value))
                        throw unbouded_variable("unbouded_variable:" + name);    
                } else
                    throw unbouded_variable("unbouded_variable:" + name);
            }
        } else {
            e->env_map = alloc<Environment_Map>(name,value);
        }            
    }

    void env_map_show(Environment_Map *a)
    {
        cerr << a->name << " l ";
        if(a->l)
            cerr << a->l->name  << " " << (char*)a->l->val - mem[0];
        cerr << " g ";
        if(a->g)
            cerr << a->g->name << " " << (char*)a->g->val - mem[0];
        cerr << endl;
        if(a->l) {
            env_map_show(a->l);
        }
        if(a->g) {
            env_map_show(a->g);
        }
    }
}
