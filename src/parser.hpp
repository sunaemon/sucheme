#pragma once
#include <tuple>
#include <string>
#include <memory>
#include "exceptions.hpp"
#include "generic_functions.hpp"
#include "lispval.hpp"

namespace sucheme {
    using std::shared_ptr;
    using std::string;

    std::tuple<shared_ptr<LispVal>, int> PExpr(const string &s, int32_t p = 0);
    std::shared_ptr<LispVal> parse(const string &s);
}
