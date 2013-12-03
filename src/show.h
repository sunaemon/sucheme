#pragma once

#include "gc_objects.h"

IF_CPP(extern "C" {)

char *showptr(const GCPtr val);
char *show(const GCPtr val);
char *memory_location(void *ptr);

IF_CPP(})
