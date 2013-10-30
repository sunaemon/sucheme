#pragma once

#include <vector>
#include <tuple>
#include <list>
#include <functional>
#include <string>
#include <memory>

namespace sucheme {
    using std::unique_ptr;
    using std::string;
    using std::vector;
    using std::move;


    template <class T, class ...Args>
    inline std::unique_ptr<T> make_unique(Args &&...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    struct LispVal
    {
        virtual string show() const = 0;
        virtual unique_ptr<LispVal> eval() = 0;
        virtual unique_ptr<LispVal> clone()  const = 0;
        virtual bool operator==(const LispVal&) const = 0;
    };

    struct Number : LispVal
    {
        int integer;
        string show() const override;
        unique_ptr<LispVal> clone() const override {
            return make_unique<Number>(integer);
        }
        unique_ptr<LispVal> eval() override {
            return clone();
        }
        bool operator==(const LispVal &val) const {
            auto a = dynamic_cast<const Number*>(&val);
            return a && a->integer == integer;
        }

        Number(int integer) : integer(integer) {}
    };

    struct Symbol : LispVal
    {
        string name;
        string show() const override;
        unique_ptr<LispVal> clone() const override {
            return make_unique<Symbol>(name);
        }
        unique_ptr<LispVal> eval() override;

        bool operator==(const LispVal&val) const {
            auto a = dynamic_cast<const Symbol*>(&val);
            return a && a->name == name;
        }

        Symbol(const string &name) : name(name) {}
    };

    struct Pair : LispVal
    {
        unique_ptr<LispVal> car;
        unique_ptr<LispVal> cdr;

        string show() const override;
        unique_ptr<LispVal> eval() override;

        unique_ptr<LispVal> clone() const override {
            auto ret = make_unique<Pair>();
            ret->car = car->clone();
            ret->cdr = cdr->clone();
            return move(ret);
        }

        bool operator==(const LispVal&val) const {
            auto a = dynamic_cast<const Pair*>(&val);
            return a && a->car == car && a->cdr == cdr;
        }

//        Pair(LispVal &&car, LispVal &&cdr) : car(car), cdr(cdr) {}
//        Pair(LispVal &car, LispVal &cdr) : car(car), cdr(cdr) {}
    };

    struct Empty : LispVal
    {
        string show () const override { return "()"; }
        unique_ptr<LispVal> clone() const override {
            return make_unique<Empty>();
        }
        unique_ptr<LispVal> eval() override {
            return clone();
        }

        bool operator==(const LispVal&val) const {
            auto a = dynamic_cast<const Empty*>(&val);
            return a;
        }

        Empty() {}
    };

    using subr = unique_ptr<LispVal> (*)(const vector<unique_ptr<LispVal> >&);

    struct Procedure : LispVal
    {
        subr func;
        
        string show() const override;
        unique_ptr<LispVal> clone() const override {
            return make_unique<Procedure>(func);
        }
        
        unique_ptr<LispVal> eval() override {
            return clone();
        }

        bool operator==(const LispVal&val) const {
            auto a = dynamic_cast<const Procedure*>(&val);
            return a && a->func == func;
        }

        Procedure(const subr &func) : func(func) {}
    };

    std::tuple<unique_ptr<LispVal>, int> PExpr(const string &s, int32_t p = 0);
    std::unique_ptr<LispVal> parse(const string &s);

}
