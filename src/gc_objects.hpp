#pragma once
#include "intern.hpp"
#include <vector>
#include <string>

namespace sucheme{
    using std::string;
    using std::vector;

    struct GCObject
    {
        virtual ~GCObject(){}
    };

    struct EnvironmentMap : GCObject
    {
        EnvironmentMap *g;
        EnvironmentMap *l;
        
        int name;
        GCObject *val;
        EnvironmentMap(int name, GCObject *val)
            : g(nullptr), l(nullptr), name(name), val(val) {}
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
        Pair() {}
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


}
