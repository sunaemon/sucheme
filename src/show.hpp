#pragma once
#include <memory>

namespace sucheme{
    using std::string;

    struct GCObject;

    string showptr(const GCObject *val);
    string show(const GCObject *val);
    string memory_location(void *ptr);
}
