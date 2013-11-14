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

    shared_ptr<LispVal> add(const vector<shared_ptr<LispVal> > &arg);
    shared_ptr<LispVal> eq(const vector<shared_ptr<LispVal> > &arg);
    shared_ptr<LispVal> sub(const vector<shared_ptr<LispVal> > &arg);
    shared_ptr<LispVal> mul(const vector<shared_ptr<LispVal> > &arg);
    shared_ptr<LispVal> print(const vector<shared_ptr<LispVal> > &arg);
    shared_ptr<LispVal> car(const vector<shared_ptr<LispVal> > &arg);
    shared_ptr<LispVal> cdr(const vector<shared_ptr<LispVal> > &arg);
    shared_ptr<LispVal> null_is(const vector<shared_ptr<LispVal> > &arg);
}
