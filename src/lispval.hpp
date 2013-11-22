#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include "exceptions.hpp"
#include "intern.hpp"
#include "gc.hpp"

namespace sucheme {
    using std::string;
    using std::vector;

    struct Environment;

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
