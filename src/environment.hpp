#pragma once
#include "parser.hpp"
#include <map>
#include <exception>
#include <string>
#include "exceptions.hpp"
#include <iostream>

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

    struct Environment_Map : LispVal
    {
        Environment_Map *g;
        Environment_Map *l;
        
        string name;
        LispVal *val;
        Environment_Map(string name, LispVal *val)
            : g(nullptr), l(nullptr), name(name), val(val) {}
    };

    struct Environment : LispVal {
        Environment *parent;

        Environment_Map *env_map;

        Environment(Environment *parent)
        : parent(parent), env_map(nullptr) {}
    };

    bool env_have(const Environment *e, const string &name);
    LispVal *env_lookup(const Environment *e, const string &name);
    void env_define(Environment *e, const string &name, LispVal *value);
    void env_set(Environment *e, const string &name, LispVal *value);

    template<typename Callback>
    void env_map_foreach(const Callback &c, Environment_Map *a) {
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

    void env_map_show(Environment_Map *a);
}
