#pragma once
#include "intern.hpp"
#include <vector>
#include <string>
#include "exceptions.hpp"
#include <sstream>
#include "show.hpp"

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


    struct GCObject;
    using subr = GCObject *(*)(const vector<GCObject*>&);
    typedef GCObject *GCPtr;

    struct GCObject
    {
        char tag;

        union {
            struct {
                GCPtr whereis;
            } destructed;
            struct {
                GCPtr g;
                GCPtr l;
                int id;
                GCPtr val;
            } map;
            struct {
                GCPtr parent;    //Environment
                GCPtr env_map;   //EnvironmentMap
            } env;
            struct {
                int integer;
            } num;
            struct {
                bool value;
            } b;
            struct {
                int id;
            } symbol;
            struct {
                subr func;
            } proc;
            struct {
                GCPtr car;
                GCPtr cdr;
            } pair;
            struct {
                vector<int> formals;
                GCPtr body;        //Pair
                GCPtr environment; //Environment
            } lambda;
        };
    };

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

#define dcast_spec(T) \
    inline GCPtr dcast_##T(GCPtr val) {\
        if(val->tag == TAG_##T)\
            return val;\
        else\
            return nullptr;\
    }\
    inline GCPtr dcast_const_##T(const GCPtr val) {\
        if(val->tag == TAG_##T)\
            return val;\
        else\
            return nullptr;\
    }

    define_for_all(dcast_spec)

#define dcast_ex_spec(T) \
    inline GCPtr dcast_ex_##T(GCPtr a) {\
        GCPtr ret = dcast_##T(a);\
        if(ret)\
            return ret;\
        else {\
            std::stringstream ost;\
            ost <<  "bad_cast: tried to convert " << (int)a->tag << " to " << #T << "\n";\
            ost << "value:" << show(a);\
            throw bad_lisp_cast(ost.str());\
        }\
    }

    define_for_all(dcast_ex_spec)

    inline void init_EnvironmentMap(GCPtr i, int id, GCPtr val)
    {
        i->tag = TAG_EnvironmentMap;
        i->map.g = nullptr;
        i->map.l = nullptr;
        i->map.id = id;
        i->map.val = val;
    }

    inline void init_Environment(GCPtr i, GCPtr parent)
    {
        i->tag = TAG_Environment;
        i->env.parent = parent;
        i->env.env_map = nullptr;
    }

    inline void init_Number(GCPtr i, int integer)
    {
        i->tag = TAG_Number;
        i->num.integer = integer;
    }

    inline void init_Bool(GCPtr i, bool value)
    {
        i->tag = TAG_Bool;
        i->b.value = value;
    }

    inline void init_Symbol(GCPtr i, const string &name)
    {
        i->tag = TAG_Symbol;
        i->symbol.id = intern_symbol(name.c_str());
    }

    inline void init_Symbol(GCPtr i, const char *name)
    {
        i->tag = TAG_Symbol;
        i->symbol.id = intern_symbol(name);
    }

    inline void init_Symbol(GCPtr i, int id)
    {
        i->tag = TAG_Symbol;
        i->symbol.id = id;
    }

    inline void init_Empty(GCPtr i)
    {
        i->tag = TAG_Empty;
    }

    inline void init_Procedure(GCPtr i, const subr &func)
    {
        i->tag = TAG_Procedure;
        i->proc.func = func;
    }

    inline void init_Pair(GCPtr i, GCPtr car, GCPtr cdr)
    {
        i->tag = TAG_Pair;
        i->pair.car = car;
        i->pair.cdr = cdr;
    }

    inline void init_LambdaProcedure(GCPtr i, const vector<int> &formals,
                                     GCPtr body,
                                     GCPtr environment)
    {
        dcast_ex_Pair(body);
        dcast_ex_Environment(body);
        i->tag = TAG_LambdaProcedure;
        i->lambda.formals = formals;
        i->lambda.body = body;
        i->lambda.environment = environment;
    }

    inline void init_LambdaMacro(GCPtr i, const vector<int> &formals,
                                 GCPtr body,
                                 GCPtr environment)
    {
        dcast_ex_Pair(body);
        dcast_ex_Environment(body);
        i->tag = TAG_LambdaMacro;
        i->lambda.formals = formals;
        i->lambda.body = body;
        i->lambda.environment = environment;
    }


#define spec_for_all_types(f) f(EnvironmentMap)f(Environment)f(Number)f(Bool)f(Symbol)f(Empty)f(Procedure)f(Pair)f(LabdaProcedure)f(LambdaMacro)

    


}
