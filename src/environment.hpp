#pragma once
#include "parser.hpp"
#include <map>
#include <exception>
#include <string>

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
        Environment *parent;

        map<string, shared_ptr<LispVal> > env_map;

        shared_ptr<LispVal> lookup(const string &s) const {
            auto i = env_map.find(s);

            if(i == env_map.end()) {
                if(parent)
                    return parent->lookup(s);
                else
                    throw std::exception();
            } else {
                return (*i).second;
            }
        }
    };
}
