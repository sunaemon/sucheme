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

    struct GCObject
    {
        virtual ~GCObject(){}
        GCObject *whereis;
    };

    struct EnvironmentMap : GCObject
    {
        EnvironmentMap *g;
        EnvironmentMap *l;
        
        int id;
        GCObject *val;
        EnvironmentMap(int id, GCObject *val)
            : g(nullptr), l(nullptr), id(id), val(val) {}
    };

    struct Environment : GCObject {
        Environment *parent;

        EnvironmentMap *env_map;

        Environment(Environment *parent)
        : parent(parent), env_map(nullptr) {}
    };

    struct Number : GCObject
    {
        int integer;

        Number(int integer) : integer(integer) {}
    };

    struct Bool : GCObject
    {
        bool value;

        Bool(bool value) : value(value) {}
    };

    struct Symbol : GCObject
    {
        int id;

        Symbol(const string &name) : id(intern_symbol(name.c_str())) {}
        Symbol(const char *name) : id(intern_symbol(name)) {}
        Symbol(int id) : id(id) {}
    };

    struct Empty : GCObject
    {
        Empty() {}
    };

    struct Procedure : GCObject
    {
        using subr = GCObject *(*)(const vector<GCObject*>&);

        subr func;
        
        GCObject *call(const vector<GCObject*> &param) {
            return func(param);
        }

        Procedure(const subr &func) : func(func) {}
    };

    struct Pair : GCObject
    {
        GCObject *car;
        GCObject *cdr;

        Pair(GCObject *car, GCObject *cdr) : car(car), cdr(cdr) {}
        Pair(): car(nullptr), cdr(nullptr) {}
    };

    struct LambdaProcedure : GCObject
    {
        vector<int> formals;
        Pair *body;
        Environment *environment;
        
        LambdaProcedure(const vector<int> &formals,
                        Pair *body,
                        Environment *environment) :
            formals(formals), body(body), environment(environment) {}
    };
    struct LambdaMacro : GCObject
    {
        vector<int> formals;
        Pair *body;
        Environment *environment;
        
        LambdaMacro(const vector<int> &formals,
                        Pair *body,
                        Environment *environment) :
            formals(formals), body(body), environment(environment) {}
    };

    template<typename T0,typename T1> T0* dcast(T1 *a)
    {
        return dynamic_cast<T0*>(a);
    }

    template<typename T0,typename T1> const T0* dcast_const(const T1 *a)
    {
        return dynamic_cast<const T0*>(a);
    }

    template<typename T0,typename T1> inline T0* dcast_ex(T1 *a)
    {
        auto ret = dynamic_cast<T0*>(a);
        if(ret)
            return std::move(ret);
        else {
            std::stringstream ost;
            ost <<  "bad_cast: tried to convert " << typeid(*a).name() << " to " << typeid(T0).name() << "\n";
            ost << "value:" << show(a);
            throw bad_lisp_cast(ost.str());
        }
    }
}
