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
    GCPtr add(unsigned int argc, const GCPtr argv[]);
    GCPtr eq(unsigned int argc, const GCPtr argv[]);
    GCPtr sub(unsigned int argc, const GCPtr argv[]);
    GCPtr mul(unsigned int argc, const GCPtr argv[]);
    GCPtr print(unsigned int argc, const GCPtr argv[]);
    GCPtr car(unsigned int argc, const GCPtr argv[]);
    GCPtr cdr(unsigned int argc, const GCPtr argv[]);
    GCPtr null_is(unsigned int argc, const GCPtr argv[]);
}
