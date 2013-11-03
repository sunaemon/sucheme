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
}
