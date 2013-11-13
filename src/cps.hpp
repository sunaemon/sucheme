#pragma once
#include "lispval.hpp"

namespace sucheme{
    shared_ptr<LispVal> cps(const shared_ptr<LispVal> &program, const shared_ptr<LispVal> &cont, const Environment &e);
    bool is_simple(const shared_ptr<LispVal> &program, const Environment &e);

}
