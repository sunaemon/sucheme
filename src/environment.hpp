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

    struct Environment {
        shared_ptr<Environment> parent;

        map<string, shared_ptr<LispVal> > env_map;

        Environment(const shared_ptr<Environment> &parent) : parent(parent) {}
    };

    void set(Environment *e, const string &name, const shared_ptr<LispVal> &value);
    bool have(const Environment *e, const string &name);
    shared_ptr<LispVal> lookup(const Environment *e, const string &name);
    void define(Environment *e, const string &name, const shared_ptr<LispVal> &value);
}
