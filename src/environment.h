#pragma once
#include "parser.h"
#include "exceptions.h"
#include "gc_objects.h"

IF_CPP(extern "C" {)

typedef struct
{
    bool found;
    GCPtr val;
} env_map_find_return;

env_map_find_return env_map_find(const EnvironmentMap *a, int id);
bool env_have(const Environment *e, int id);
GCPtr env_lookup(const Environment *e, int id);
bool env_have(const Environment *e, int name);
void env_define(Environment *e, int name, GCPtr value);
void env_set(Environment *e, int name, GCPtr value);
void env_intern_define(Environment *e, const char *name, GCPtr value);
void env_map_foreach(void (*c)(int,GCPtr), EnvironmentMap *a);
void env_foreach(void (*c)(int,GCPtr), Environment *e);


void env_map_show(EnvironmentMap *a);

IF_CPP(})
