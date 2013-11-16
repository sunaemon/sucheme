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

    struct Environment_Map
    {
        shared_ptr<Environment_Map> g;
        shared_ptr<Environment_Map> l;
        
        string name;
        shared_ptr<LispVal> val;
    };

    struct Environment {
        shared_ptr<Environment> parent;

        Environment_Map env_map;

        Environment(const shared_ptr<Environment> &parent) : parent(parent) {}
    };

    bool env_have(const Environment *e, const string &name);
    shared_ptr<LispVal> env_lookup(const Environment *e, const string &name);
    void env_define(Environment *e, const string &name, const shared_ptr<LispVal> &value);
    void env_set(Environment *e, const string &name, const shared_ptr<LispVal> &value);
}
