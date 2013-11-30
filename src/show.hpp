#pragma once
#include <memory>

namespace sucheme{
    using std::string;

    struct GCObject;
    typedef GCObject *GCPtr;

    string showptr(const GCPtr val);
    string show(const GCPtr val);
    string memory_location(void *ptr);
}
