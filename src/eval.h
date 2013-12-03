#pragma once
#include "gc_objects.h"

IF_CPP(extern "C" {)

GCPtr eval(GCPtr a, Environment *e);

IF_CPP(})
