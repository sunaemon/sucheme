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

    LispVal* add(const vector<LispVal* > &arg);
    LispVal* eq(const vector<LispVal* > &arg);
    LispVal* sub(const vector<LispVal* > &arg);
    LispVal* mul(const vector<LispVal* > &arg);
    LispVal* print(const vector<LispVal* > &arg);
    LispVal* car(const vector<LispVal* > &arg);
    LispVal* cdr(const vector<LispVal* > &arg);
    LispVal* null_is(const vector<LispVal* > &arg);
}
