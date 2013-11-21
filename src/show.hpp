#pragma once
#include <memory>
#include "lispval.hpp"
namespace sucheme{
    using std::string;

    string show(const LispVal *val);
}
