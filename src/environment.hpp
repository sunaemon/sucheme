#pragma once
#include "parser.hpp"
#include <map>
#include <exception>
#include "exceptions.hpp"
#include <iostream>
#include "intern.hpp"

namespace sucheme
{
    using std::get;
    using std::move;
    using std::unique_ptr;
    using std::map;
    using std::cerr;
    using std::endl;

    struct GCObject;

    struct EnvironmentMap : GCObject
    {
        EnvironmentMap *g;
        EnvironmentMap *l;
        
        int name;
        GCObject *val;
        EnvironmentMap(int name, GCObject *val)
            : g(nullptr), l(nullptr), name(name), val(val) {}
    };

    struct Environment : GCObject {
        Environment *parent;

        EnvironmentMap *env_map;

        Environment(Environment *parent)
        : parent(parent), env_map(nullptr) {}
    };

    bool env_have(const Environment *e, int name);
    GCObject *env_lookup(const Environment *e, int name);
    void env_define(Environment *e, int name, GCObject *value);
    void env_set(Environment *e, int name, GCObject *value);
    inline void env_intern_define(Environment *e, const char *name, GCObject *value) {
        env_define(e,intern_symbol(name), value);
    }


    template<typename Callback>
    void env_map_foreach(const Callback &c, EnvironmentMap *a) {
        if(a->l)
            env_map_foreach(c,a->l);

        c(a->name,a->val);

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
