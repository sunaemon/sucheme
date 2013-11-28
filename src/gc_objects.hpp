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
        TAG_LambdaMacro,
    };

    struct GCObject
    {
        char tag;
        GCObject *whereis;
        GCObject(char tag) : tag(tag) {}
    };

    struct EnvironmentMap : GCObject
    {
        EnvironmentMap *g;
        EnvironmentMap *l;
        
        int id;
        GCObject *val;
        EnvironmentMap(int id, GCObject *val)
            : GCObject(TAG_EnvironmentMap), g(nullptr), l(nullptr), id(id), val(val) {}
    };

    struct Environment : GCObject {
        Environment *parent;

        EnvironmentMap *env_map;

        Environment(Environment *parent)
        : GCObject(TAG_Environment), parent(parent), env_map(nullptr) {}
    };

    struct Number : GCObject
    {
        int integer;

        Number(int integer) : GCObject(TAG_Number), integer(integer) {}
    };

    struct Bool : GCObject
    {
        bool value;

        Bool(bool value) : GCObject(TAG_Bool), value(value) {}
    };

    struct Symbol : GCObject
    {
        int id;

        Symbol(const string &name) : GCObject(TAG_Symbol), id(intern_symbol(name.c_str())) {}
        Symbol(const char *name) : GCObject(TAG_Symbol), id(intern_symbol(name)) {}
        Symbol(int id) : GCObject(TAG_Symbol), id(id) {}
    };

    struct Empty : GCObject
    {
        Empty() : GCObject(TAG_Empty) {}
    };

    struct Procedure : GCObject
    {
        using subr = GCObject *(*)(const vector<GCObject*>&);

        subr func;
        
        GCObject *call(const vector<GCObject*> &param) {
            return func(param);
        }

        Procedure(const subr &func) : GCObject(TAG_Procedure), func(func) {}
    };

    struct Pair : GCObject
    {
        GCObject *car;
        GCObject *cdr;

        Pair(GCObject *car, GCObject *cdr) : GCObject(TAG_Pair), car(car), cdr(cdr) {}
        Pair(): GCObject(TAG_Pair), car(nullptr), cdr(nullptr) {}
    };

    struct LambdaProcedure : GCObject
    {
        vector<int> formals;
        Pair *body;
        Environment *environment;
        
        LambdaProcedure(const vector<int> &formals,
                        Pair *body,
                        Environment *environment) :
            GCObject(TAG_LambdaProcedure), formals(formals), body(body), environment(environment) {}
    };
    struct LambdaMacro : GCObject
    {
        vector<int> formals;
        Pair *body;
        Environment *environment;
        
        LambdaMacro(const vector<int> &formals,
                        Pair *body,
                        Environment *environment) :
            GCObject(TAG_LambdaMacro), formals(formals), body(body), environment(environment) {}
    };

    template<typename T0,typename T1> T0* dcast(T1 *)
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
    f(LambdaProcedure)\
    f(LambdaMacro)

#define dcast_spec(T0) \
    template<> inline T0* dcast<T0, GCObject>(GCObject *a) \
    {\
    if(TAG_##T0 == a->tag)\
        return (T0*)a;\
    else\
        return nullptr;\
    }

    define_for_all(dcast_spec)

    template<typename T0,typename T1> const T0* dcast_const(const T1 *)
    {
        throw not_implemented();
    }

#define dcast_const_spec(T0) \
    template<> inline const T0* dcast_const<T0, GCObject>(const GCObject *a) \
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
}
