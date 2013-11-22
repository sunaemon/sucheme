#pragma once
#include <memory>
#include "lispval.hpp"
#include "gc.hpp"

namespace sucheme{
    using std::string;

    string showptr(const GCObject *val);
    string show(const GCObject *val);
}
