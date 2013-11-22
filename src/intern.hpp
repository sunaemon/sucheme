#pragma once
#include <string>

namespace sucheme
{
    using std::string;
    enum {
        ID_LAMBDA,
        ID_COND,
        ID_QUOTE,
        ID_DEFINE,
        ID_DEFINE_MACRO,
        ID_SET,
        ID_BEGIN,
        REGISTERED_ID_COUNT
    };

    int intern_symbol(const char *name);
    string extern_symbol(int id);
}
