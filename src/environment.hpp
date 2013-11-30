#pragma once
#include "parser.hpp"
#include "exceptions.hpp"
#include "intern.hpp"
#include "gc_objects.hpp"

namespace sucheme
{
    struct env_map_find_return
    {
        bool found;
        GCPtr val;
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

    inline GCPtr env_lookup(const Environment *e, int id)  {
        if(e->env_map) {
            auto ret = env_map_find(e->env_map, id);
            if(ret.found)
                return ret.val;
        }
        
        if(e->parent)
            return env_lookup(e->parent, id);
        else {
            sprintf(ex_buf, "unbouded_variable:%s", extern_symbol(id));
            throw unbouded_variable(ex_buf);
        }
    }

    bool env_have(const Environment *e, int name);
    void env_define(Environment *e, int name, GCPtr value);
    void env_set(Environment *e, int name, GCPtr value);
    inline void env_intern_define(Environment *e, const char *name, GCPtr value) {
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
