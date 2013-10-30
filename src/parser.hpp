#pragma once

#include <vector>
#include <tuple>
#include <list>
#include <functional>
#include <string>
#include <memory>

namespace sucheme {
    struct LispVal
    {
        virtual std::string show() = 0;
//    virtual LispVal eval() = 0;
    };

    struct Number : LispVal
    {
        int integer;
        std::string show() override;
//    LispVal eval() override;
        Number(int integer) : integer(integer) {}
    };

    struct Symbol : LispVal
    {
        std::string name;
        std::string show() override;
//    LispVal eval() override { return value_map[name]; }
        Symbol(const std::string &name) : name(name) {}
    };

    struct Pair : LispVal
    {
        std::unique_ptr<LispVal> car;
        std::unique_ptr<LispVal> cdr;

        std::string show() override;

//        Pair(LispVal &&car, LispVal &&cdr) : car(car), cdr(cdr) {}
//        Pair(LispVal &car, LispVal &cdr) : car(car), cdr(cdr) {}
    };

    struct Empty : LispVal
    {
        std::string show () override { return "()"; }
        Empty() {}
    };

    struct Procedure : LispVal
    {
//        std::string show() override;
//    LispVal eval() override {return this};
    };

    std::tuple<std::unique_ptr<LispVal>, int> PExpr(const std::string &s, int32_t p = 0);
}
