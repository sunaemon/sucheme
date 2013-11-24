#include "environment.hpp"
#include "gc.hpp"

namespace sucheme
{
    using std::cerr;
    using std::endl;
    using std::to_string;

    struct GCObject;

    bool env_map_set(EnvironmentMap *a, int id, GCObject *val)
    {
        EnvironmentMap *next = a;
        for(;;) {
            if(id < next->id) {
                if(next->l)
                    next=next->l;
                else
                    return false;
            } else if(id > next->id) {
                if(next->g)
                    next=next->g;
                else
                    return false;
            } else if(id == next->id){
                next->val = val;
                return true;
            }
        }
    }

    void env_map_add(EnvironmentMap *a, int id, GCObject *val)
    {
        EnvironmentMap *next = a;
        for(;;) {
            if(id < next->id) {
                if(next->l)
                    next=next->l;
                else {
                    next->l = alloc<EnvironmentMap>(id,val);
                    return;
                }
            } else if(id > next->id) {
                if(next->g)
                    next=next->g;
                else {
                    next->g = alloc<EnvironmentMap>(id,val);
                    return;
                }
            } else if(id == next->id){
                if(val)
                    next->val = val;
                return;
            }
        }
    }


    void env_define(Environment *e, int id, GCObject *value) {
        if(e->env_map)
            env_map_add(e->env_map, id, value); // copy
        else {
            e->env_map = alloc<EnvironmentMap>(id,value);
        }
    }

    void env_set(Environment *e, int id, GCObject *value) {
        if(e->env_map) {
            if(!env_map_set(e->env_map, id, value)){
                if(e->parent) {
                    if(!env_map_set(e->env_map, id, value))
                        throw unbouded_variable("unbouded_variable:" + to_string(id));
                } else
                    throw unbouded_variable("unbouded_variable:" + to_string(id));
            }
        } else {
            e->env_map = alloc<EnvironmentMap>(id,value);
        }            
    }

    void env_map_show(EnvironmentMap *a)
    {
        cerr << a->id << " l ";
        if(a->l)
            cerr << a->l->id  << " " << (char*)a->l->val - mem[0];
        cerr << " g ";
        if(a->g)
            cerr << a->g->id << " " << (char*)a->g->val - mem[0];
        cerr << endl;
        if(a->l) {
            env_map_show(a->l);
        }
        if(a->g) {
            env_map_show(a->g);
        }
    }
}
