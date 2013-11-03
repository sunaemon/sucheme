#pragma once
#include <vector>
#include <tuple>
#include <list>
#include <functional>
#include <string>
#include <memory>

namespace sucheme {
    using std::unique_ptr;
    using std::shared_ptr;
    using std::string;
    using std::vector;
    using std::move;
    using std::make_shared;
    using std::make_tuple;
    using std::get;
    using std::dynamic_pointer_cast;


    template <class T, class ...Args>
    inline std::unique_ptr<T> make_unique(Args &&...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    struct Environment;

    struct LispVal
    {
        virtual string show() const = 0;
        virtual shared_ptr<LispVal> eval(const Environment &) = 0;
        virtual bool operator==(const LispVal&) const = 0;
    };

    struct Number : LispVal, std::enable_shared_from_this<Number>
    {
        int integer;
        string show() const override;
        shared_ptr<LispVal> eval(const Environment &) override {
            return shared_from_this();
        }
        bool operator==(const LispVal &val) const {
            auto a = dynamic_cast<const Number*>(&val);
            return a && a->integer == integer;
        }

        Number(int integer) : integer(integer) {}
    };

    struct Symbol : LispVal, std::enable_shared_from_this<Symbol>
    {
        string name;
        string show() const override;
        shared_ptr<LispVal> eval(const Environment &) override;

        bool operator==(const LispVal&val) const {
            auto a = dynamic_cast<const Symbol*>(&val);
            return a && a->name == name;
        }

        Symbol(const string &name) : name(name) {}
    };

    struct Pair : LispVal, std::enable_shared_from_this<Pair>
    {
        shared_ptr<LispVal> car;
        shared_ptr<LispVal> cdr;

        string show() const override;
        shared_ptr<LispVal> eval(const Environment &) override;

        bool operator==(const LispVal &val) const {
            auto a = dynamic_cast<const Pair*>(&val);
            return a && a->car == car && a->cdr == cdr;
        }

//        Pair(LispVal &&car, LispVal &&cdr) : car(car), cdr(cdr) {}
//        Pair(LispVal &car, LispVal &cdr) : car(car), cdr(cdr) {}
    };

    struct Empty : LispVal, std::enable_shared_from_this<Empty>
    {
        string show () const override { return "()"; }
        shared_ptr<LispVal> eval(const Environment &) override {
            return shared_from_this();
        }

        bool operator==(const LispVal&val) const {
            auto a = dynamic_cast<const Empty*>(&val);
            return a;
        }

        Empty() {}
    };

    using subr = shared_ptr<LispVal> (*)(const vector<shared_ptr<LispVal> >&);

    struct Procedure : LispVal, std::enable_shared_from_this<Procedure>
    {
        subr func;
        
        string show() const override;
        
        shared_ptr<LispVal> eval(const Environment &) override {
            return shared_from_this();
        }

        bool operator==(const LispVal &val) const {
            auto a = dynamic_cast<const Procedure*>(&val);
            return a && a->func == func;
        }

        Procedure(const subr &func) : func(func) {}
    };

    std::tuple<shared_ptr<LispVal>, int> PExpr(const string &s, int32_t p = 0);
    std::shared_ptr<LispVal> parse(const string &s);

}
