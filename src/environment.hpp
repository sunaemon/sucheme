#pragma once
#include "parser.hpp"
#include <map>
#include <exception>
#include <string>
#include "exceptions.hpp"

namespace sucheme
{
    using std::string;
    using std::ostringstream;
    using std::get;
    using std::move;
    using std::unique_ptr;
    using std::map;

    struct LispVal;

    struct Environment {
        shared_ptr<Environment> parent;

        map<string, shared_ptr<LispVal> > env_map;

        bool have(const string &name) const {
            return env_map.find(name) != env_map.end();
        }

        shared_ptr<LispVal> lookup(const string &name) const {
            auto i = env_map.find(name);

            if(i != env_map.end()) {
                return (*i).second;
            } else {
                if(parent)
                    return parent->lookup(name);
                else
                    throw unbouded_variable("unbouded_variable:" + name);
            }
        }
        
        void define(const string &name, const shared_ptr<LispVal> &value) {
            env_map[name] = value; //copy
        }

        Environment(const shared_ptr<Environment> parent) : parent(parent) {}
    };
}
