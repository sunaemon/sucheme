#pragma once
#include "macro.h"

IF_CPP(extern "C" {)

enum {
    ID_LAMBDA,
    ID_COND,
    ID_QUOTE,
    ID_DEFINE,
    ID_DEFINE_MACRO,
    ID_SET,
    ID_BEGIN,
    ID_ELSE,
    REGISTERED_ID_COUNT
};

int intern_symbol(const char *name);
const char *extern_symbol(int id);

IF_CPP(})
