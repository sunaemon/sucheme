#pragma once
#include <memory>
#include "lispval.hpp"
namespace sucheme{
    using std::shared_ptr;

    shared_ptr<LispVal> eval(const shared_ptr<LispVal> &a, shared_ptr<Environment> &e);
}
