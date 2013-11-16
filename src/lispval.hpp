#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include "exceptions.hpp"

namespace sucheme {
    using std::string;
    using std::vector;

    struct Environment;

    struct LispVal
    {
        virtual ~LispVal(){}
    };

    struct Number : LispVal
    {
        int integer;

        Number(int integer) : integer(integer) {}
    };

    struct Bool : LispVal
    {
        bool value;

        Bool(bool value) : value(value) {}
    };


    struct Symbol : LispVal
    {
        string name;

        Symbol(const string &name) : name(name) {}
    };

    struct Pair : LispVal
    {
        LispVal *car;
        LispVal *cdr;

        Pair(LispVal *car, LispVal *cdr) : car(car), cdr(cdr) {}
        Pair() {}
    };

    struct Empty : LispVal
    {
        Empty() {}
    };

    struct Procedure : LispVal
    {
        using subr = LispVal *(*)(const vector<LispVal*>&);

        subr func;
        
        LispVal *call(const vector<LispVal*> &param) {
            return func(param);
        }

        Procedure(const subr &func) : func(func) {}
    };

    struct LambdaProcedure : LispVal
    {
        vector<string> formals;
        Pair *body;
        Environment *environment;
        
        LambdaProcedure(const vector<string> &formals,
                        Pair *body,
                        Environment *environment) :
            formals(formals), body(body), environment(environment) {}
    };
    struct LambdaMacro : LispVal
    {
        vector<string> formals;
        Pair *body;
        Environment *environment;
        
        LambdaMacro(const vector<string> &formals,
                        Pair *body,
                        Environment *environment) :
            formals(formals), body(body), environment(environment) {}
    };
}
