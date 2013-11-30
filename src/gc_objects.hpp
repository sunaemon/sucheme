#pragma once
#include "intern.hpp"
#include "exceptions.hpp"
#include "show.hpp"
#include <stdlib.h>

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
};

struct GCObject
{
    char tag;
    GCObject *whereis;
    GCObject(char tag) : tag(tag), whereis(nullptr) {}
};

typedef GCObject *GCPtr;

struct EnvironmentMap
{
    GCObject obj;

    EnvironmentMap *g;
    EnvironmentMap *l;
        
    int id;
    GCPtr val;
    EnvironmentMap(int id, GCPtr val)
        : obj(TAG_EnvironmentMap), g(nullptr), l(nullptr), id(id), val(val) {}
};

struct Environment
{
    GCObject obj;

    Environment *parent;

    EnvironmentMap *env_map;

    Environment(Environment *parent)
    : obj(TAG_Environment), parent(parent), env_map(nullptr) {}
};

struct Number
{
    GCObject obj;
    int integer;

    Number(int integer) : obj(TAG_Number), integer(integer) {}
};

struct Bool
{
    GCObject obj;
    bool value;

    Bool(bool value) : obj(TAG_Bool), value(value) {}
};

struct Symbol
{
    GCObject obj;
    int id;

    Symbol(const char *name) : obj(TAG_Symbol), id(intern_symbol(name)) {}
    Symbol(int id) : obj(TAG_Symbol), id(id) {}
};

struct Empty
{
    GCObject obj;
    Empty() : obj(TAG_Empty) {}
};

struct Procedure
{
    GCObject obj;
    using subr = GCPtr (*)(unsigned int argc, const GCPtr argv[]);

    subr func;
        
    GCPtr call(unsigned int argc, const GCPtr argv[]) {
        return func(argc, argv);
    }

    Procedure(const subr &func) : obj(TAG_Procedure), func(func) {}
};

struct Pair
{
    GCObject obj;
    GCPtr car;
    GCPtr cdr;

    Pair(GCPtr car, GCPtr cdr) : obj(TAG_Pair), car(car), cdr(cdr) {}
    Pair(): obj(TAG_Pair), car(nullptr), cdr(nullptr) {}
};

#define LAMBDA_MAX_ARG 10

struct LambdaProcedure
{
    GCObject obj;
    int argc;
    int argv[LAMBDA_MAX_ARG];
    GCPtr body;
    Environment *environment;
        
    LambdaProcedure(GCPtr body,
                    Environment *environment) :
        obj(TAG_LambdaProcedure), body(body), environment(environment) {}
};

template<typename T0> T0* dcast(GCPtr)
{
    sprintf(ex_buf, "not_implemented");
    throw_jump();
    //return dynamic_cast<T0*>(a);
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
    template<> inline T0* dcast<T0>(GCPtr a)    \
    {                                           \
        if(TAG_##T0 == a->tag)                  \
            return (T0*)a;                      \
        else                                    \
            return nullptr;                     \
    }

define_for_all(dcast_spec)

template<typename T0> const T0* dcast_const(const GCPtr)
{
    sprintf(ex_buf, "not_implemented");
    throw_jump();
}

#define dcast_const_spec(T0)                                    \
    template<> inline const T0* dcast_const<T0>(const GCPtr a)  \
    {                                                           \
        if(TAG_##T0 == a->tag)                                  \
            return (T0*)a;                                      \
        else                                                    \
            return nullptr;                                     \
    }

define_for_all(dcast_const_spec)

template<typename T0> inline T0 *dcast_ex(GCPtr)
{
    sprintf(ex_buf, "not_implemented");
    throw_jump();
}

#define dcast_ex_spec(T0)                                               \
    template<> inline T0* dcast_ex<T0>(GCPtr a)                         \
    {                                                                   \
        auto ret = dcast<T0>(a);                                        \
        if(ret)                                                         \
            return ret;                                                 \
        else {                                                          \
            char *buf = show(a);                                        \
            sprintf(ex_buf, "bad_cast: tried to convert %d to %s, value: %s", a->tag, #T0,buf); \
            free(buf);                                                  \
            throw_jump();                                               \
        }                                                               \
    }                                                                   \

define_for_all(dcast_ex_spec)

//#define ucast(a) ((GCPtr)(a))
#define ucast(a) (reinterpret_cast<GCPtr>(a))

