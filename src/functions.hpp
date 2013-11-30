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
    GCPtr add(const vector_ptr &arg);
    GCPtr eq(const vector_ptr &arg);
    GCPtr sub(const vector_ptr &arg);
    GCPtr mul(const vector_ptr &arg);
    GCPtr print(const vector_ptr &arg);
    GCPtr car(const vector_ptr &arg);
    GCPtr cdr(const vector_ptr &arg);
    GCPtr null_is(const vector_ptr &arg);
}
