#pragma once
#include <memory>
#include "lispval.hpp"
#include "gc.hpp"

namespace sucheme{
    using std::shared_ptr;

    GCObject *eval(GCObject *a, Environment *e);
}
