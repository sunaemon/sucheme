#pragma once

#include <vector>
#include <tuple>
#include <list>
#include <functional>
#include <string>

namespace sucheme {

class LispVal
{
    virtual std::string show() = 0;
//    virtual LispVal eval() = 0;
};

class Number : LispVal
{
    int integer;
    std::string show() override;
//    LispVal eval() override;
public:
    Number(int integer) : integer(integer) {}
};

class Symbol : LispVal
{
    std::string name;
    std::string show() override;
//    LispVal eval() override { return value_map[name]; }
public:
    Symbol(const std::string &name) : name(name) {}
};

class List : LispVal
{
    std::list<LispVal> list;
    std::string show() override;
//    LispVal eval() override;
public:
    List(const std::list<LispVal> list) : list(list) {}
};

class Procedure : LispVal
{
    std::string show() override;
//    LispVal eval() override {return this};
};

std::tuple<LispVal, int> PExpr(const std::string &s, int32_t p = 0);

}
