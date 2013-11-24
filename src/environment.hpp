#pragma once
#include "parser.hpp"
#include <map>
#include <exception>
#include "exceptions.hpp"
#include <iostream>
#include "intern.hpp"
#include "gc_objects.hpp"

namespace sucheme
{
    using std::get;
    using std::move;
    using std::unique_ptr;
    using std::map;
    using std::cerr;
    using std::endl;
    using std::to_string;

    struct env_map_find_return
    {
        bool found;
        GCObject *val;
    };

    inline env_map_find_return env_map_find(const EnvironmentMap *a, int id)
    {
        const EnvironmentMap *next = a;
        for(;;) {
            if(id < next->id) {
                if(next->l)
                    next=next->l;
                else
                    return {false, NULL};
            } else if(id > next->id) {
                if(next->g)
                    next=next->g;
                else
                    return {false, NULL};
            } else if(id == next->id){
                return {true, next->val};
            }
        }
    }

    inline bool env_have(const Environment *e, int id)  {
        if(e->env_map)
            return env_map_find(e->env_map, id).found;
        else
            return false;
    }

    inline GCObject *env_lookup(const Environment *e, int id)  {
        if(e->env_map) {
            auto ret = env_map_find(e->env_map, id);
            if(ret.found)
                return ret.val;
        }
        
        if(e->parent)
            return env_lookup(e->parent, id);
        else
            throw unbouded_variable("unbouded_variable:" + to_string(id));
    }

    bool env_have(const Environment *e, int name);
    void env_define(Environment *e, int name, GCObject *value);
    void env_set(Environment *e, int name, GCObject *value);
    inline void env_intern_define(Environment *e, const char *name, GCObject *value) {
        env_define(e,intern_symbol(name), value);
    }

    template<typename Callback>
    void env_map_foreach(const Callback &c, EnvironmentMap *a) {
        if(a->l)
            env_map_foreach(c,a->l);

        c(a->id,a->val);

        if(a->g)
            env_map_foreach(c,a->g);
    }

    template<typename Callback>
    void env_foreach(const Callback &c, Environment *e) {
        if(e->env_map)
            env_map_foreach(c,e->env_map);

        if(e->parent)
            env_foreach(c,e);
    }

    void env_map_show(EnvironmentMap *a);
}
