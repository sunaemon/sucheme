#pragma once
#include <memory>
#include "lispval.hpp"
namespace sucheme{
    using std::shared_ptr;

    LispVal *eval(LispVal *a, Environment *e);
}
