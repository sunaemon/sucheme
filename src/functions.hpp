#pragma once
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include "parser.hpp"
#include "environment.hpp"

namespace sucheme {
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::to_string;
    using std::unique_ptr;
    using std::shared_ptr;

    void init_environment(Environment *e);
    GCObject* add(const vector<GCObject* > &arg);
    GCObject* eq(const vector<GCObject* > &arg);
    GCObject* sub(const vector<GCObject* > &arg);
    GCObject* mul(const vector<GCObject* > &arg);
    GCObject* print(const vector<GCObject* > &arg);
    GCObject* car(const vector<GCObject* > &arg);
    GCObject* cdr(const vector<GCObject* > &arg);
    GCObject* null_is(const vector<GCObject* > &arg);
}
