#pragma once
#include "intern.hpp"
#include <vector>
#include <string>
#include <sstream>
#include "exceptions.hpp"
#include <typeinfo>

namespace sucheme{
    using std::string;
    using std::vector;

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
    typedef std::vector<GCPtr> vector_ptr;

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

        Symbol(const string &name) : obj(TAG_Symbol), id(intern_symbol(name.c_str())) {}
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
        using subr = GCPtr (*)(const vector_ptr&);

        subr func;
        
        GCPtr call(const vector_ptr &param) {
            return func(param);
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
        Pair *body;
        Environment *environment;
        
        LambdaProcedure(Pair *body,
                        Environment *environment) :
            obj(TAG_LambdaProcedure), body(body), environment(environment) {}
    };

    template<typename T0> T0* dcast(GCPtr)
    {
        throw not_implemented();
        //return dynamic_cast<T0*>(a);
    }

#define define_for_all(f) \
    f(EnvironmentMap)\
    f(Environment)\
    f(Number)\
    f(Bool)\
    f(Symbol)\
    f(Empty)\
    f(Procedure)\
    f(Pair)\
    f(LambdaProcedure)

#define dcast_spec(T0) \
    template<> inline T0* dcast<T0>(GCPtr a) \
    {\
    if(TAG_##T0 == a->tag)\
        return (T0*)a;\
    else\
        return nullptr;\
    }

    define_for_all(dcast_spec)

    template<typename T0> const T0* dcast_const(const GCPtr)
    {
        throw not_implemented();
    }

#define dcast_const_spec(T0) \
    template<> inline const T0* dcast_const<T0>(const GCPtr a) \
    {\
    if(TAG_##T0 == a->tag)\
        return (T0*)a;\
    else\
        return nullptr;\
    }

    define_for_all(dcast_const_spec)

    template<typename T0,typename T1> inline T0* dcast_ex(T1 *a)
    {
        auto ret = dcast<T0>(a);
        if(ret)
            return ret;
        else {
            std::stringstream ost;
            ost <<  "bad_cast: tried to convert " << typeid(*a).name() << " to " << typeid(T0).name() << "\n";
            ost << "value:" << show(a);
            throw bad_lisp_cast(ost.str());
        }
    }

//#define ucast(a) ((GCPtr)(a))
#define ucast(a) (reinterpret_cast<GCPtr>(a))

}
