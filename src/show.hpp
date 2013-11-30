#pragma once

namespace sucheme{
    using std::string;

    struct GCObject;
    typedef GCObject *GCPtr;

    char *showptr(const GCPtr val);
    char *show(const GCPtr val);
    char *memory_location(void *ptr);
}
