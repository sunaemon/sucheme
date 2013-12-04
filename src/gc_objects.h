#pragma once
#include "intern.h"
#include "exceptions.h"
#include <stdlib.h>
#include "macro.h"
#include <stdbool.h>

IF_CPP(extern "C" {)

enum GCObject_tag{
    TAG_Null,
    TAG_Destructed,
    TAG_Environment,
    TAG_EnvironmentMap,
    TAG_Number,
    TAG_Bool,
    TAG_Symbol,
    TAG_Empty,
    TAG_Procedure,
    TAG_Pair,
    TAG_LambdaProcedure,
    NUMBER_OF_TAG,
};

extern const char tag_name[NUMBER_OF_TAG][30];

struct GCObject_
{
    unsigned char tag;
    struct GCObject_ *whereis;
};

typedef struct GCObject_ GCObject;

inline void init_GCObject(GCObject *i, char tag)
{
    i->tag = tag;
    i->whereis = NULL;
}

typedef GCObject *GCPtr;

struct  EnvironmentMap_
{
    GCObject obj;

    struct EnvironmentMap_ *g;
    struct EnvironmentMap_ *l;
        
    int id;
    GCPtr val;
};

typedef struct EnvironmentMap_ EnvironmentMap;

inline void init_EnvironmentMap(EnvironmentMap *i, int id, GCPtr val)
{
    i->obj.tag = TAG_EnvironmentMap;
    i->g = NULL;
    i->l = NULL;
    i->id = id;
    i->val = val;
}

typedef struct Environment_
{
    GCObject obj;

    struct Environment_ *parent;

    EnvironmentMap *env_map;
}Environment;

inline void init_Environment(Environment *i, Environment *parent)
{
    i->obj.tag = TAG_Environment;
    i->parent = parent;
    i->env_map = NULL;
}

typedef struct
{
    GCObject obj;
    int integer;
}Number;

inline void init_Number(Number *i, int integer)
{
    i->obj.tag = TAG_Number;
    i->integer = integer;
}
    
typedef struct
{
    GCObject obj;
    bool value;
}Bool;

inline void init_Bool(Bool *i, bool value)
{
    i->obj.tag = TAG_Bool;
    i->value = value;
}

typedef struct
{
    GCObject obj;
    int id;
} Symbol;

inline void init_Symbol(Symbol *i, int id)
{
    i->obj.tag = TAG_Symbol;
    i->id = id;
}

typedef struct
{
    GCObject obj;
}Empty;

inline void init_Empty(Empty *i)
{
    i->obj.tag = TAG_Empty;
}

typedef GCPtr (*subr)(unsigned int argc, const GCPtr argv[]);

typedef struct
{
    GCObject obj;

    subr func;
}Procedure;

inline void init_Procedure(Procedure *i, const subr func)
{
    i->obj.tag = TAG_Procedure;
    i->func = func;
}

typedef struct
{
    GCObject obj;
    GCPtr car;
    GCPtr cdr;
}Pair;

inline void init_Pair(Pair *i, GCPtr car, GCPtr cdr)
{
    i->obj.tag = TAG_Pair;
    i->car = car;
    i->cdr = cdr;
}


#define LAMBDA_MAX_ARG 10

typedef struct
{
    GCObject obj;
    int argc;
    int argv[LAMBDA_MAX_ARG];
    GCPtr body;
    Environment *environment;
}LambdaProcedure;

inline void init_LambdaProcedure(LambdaProcedure *i, GCPtr body, Environment *e)
{
    i->obj.tag = TAG_LambdaProcedure;
    i->body = body;
    i->environment = e;
}

#define define_for_all(f)                       \
    f(EnvironmentMap)                           \
    f(Environment)                              \
        f(Number)                               \
        f(Bool)                                 \
        f(Symbol)                               \
        f(Empty)                                \
        f(Procedure)                            \
        f(Pair)                                 \
        f(LambdaProcedure)

#define dcast_spec(T0)                          \
    inline T0* dcast_##T0(GCPtr a)               \
    {                                           \
        if(TAG_##T0 == a->tag)                  \
            return (T0*)a;                      \
        else                                    \
            return NULL;                     \
    }

define_for_all(dcast_spec)

#define dcast_const_spec(T0)                                    \
    inline const T0* dcast_const_##T0(const GCPtr a)            \
    {                                                           \
        if(TAG_##T0 == a->tag)                                  \
            return (T0*)a;                                      \
        else                                                    \
            return NULL;                                     \
    }

define_for_all(dcast_const_spec)

//show.c
char *show(const GCPtr val);

#define dcast_ex_spec(T0)                                               \
    inline T0* dcast_ex_##T0(GCPtr a)                                   \
    {                                                                   \
        T0 *ret = dcast_##T0(a);                                        \
        if(ret)                                                         \
            return ret;                                                 \
        else {                                                          \
            char *buf = show(a);                                        \
            throw_jumpf("bad_cast: tried to convert %s to %s, value: %s", tag_name[(int)a->tag], #T0,buf);     \
            free(buf);                                                  \
        }                                                               \
    }                                                                   \

define_for_all(dcast_ex_spec)

#define ucast(a) ((GCPtr)(a))

IF_CPP(})

