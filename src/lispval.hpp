#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>

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

    struct Environment;

    struct LispVal
    {
        virtual string show() const = 0;
        virtual shared_ptr<LispVal> eval(shared_ptr<Environment>&) = 0;
        virtual bool operator==(const LispVal&) const = 0;
    };

    struct Number : LispVal, std::enable_shared_from_this<Number>
    {
        int integer;
        string show() const override;
        shared_ptr<LispVal> eval(shared_ptr<Environment>&) override {
            return shared_from_this();
        }
        bool operator==(const LispVal &val) const override{
            auto a = dynamic_cast<const Number*>(&val);
            return a && a->integer == integer;
        }

        Number(int integer) : integer(integer) {}
    };

    struct Bool : LispVal, std::enable_shared_from_this<Bool>
    {
        bool value;
        string show() const override { return value ? "#t" : "#b"; }
        shared_ptr<LispVal> eval(shared_ptr<Environment>&) override {
            return shared_from_this();
        }

        bool operator==(const LispVal &val) const override{
            auto a = dynamic_cast<const Bool*>(&val);
            return a && a->value == value;
        }

        Bool(bool value) : value(value) {}
    };


    struct Symbol : LispVal, std::enable_shared_from_this<Symbol>
    {
        string name;
        string show() const override;
        shared_ptr<LispVal> eval(shared_ptr<Environment>&) override;

        bool operator==(const LispVal&val) const override{
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
        shared_ptr<LispVal> eval(shared_ptr<Environment>&) override;

        bool operator==(const LispVal &val) const override{
            auto a = dynamic_cast<const Pair*>(&val);
            return a && a->car == car && a->cdr == cdr;
        }
    };

    struct Empty : LispVal, std::enable_shared_from_this<Empty>
    {
        string show () const override { return "()"; }
        shared_ptr<LispVal> eval(shared_ptr<Environment>&) override {
            return shared_from_this();
        }

        bool operator==(const LispVal&val) const override{
            auto a = dynamic_cast<const Empty*>(&val);
            return a;
        }

        Empty() {}
    };

    struct Procedure : LispVal, std::enable_shared_from_this<Procedure>
    {
        using subr = shared_ptr<LispVal> (*)(const vector<shared_ptr<LispVal> >&);

        subr func;
        
        string show() const override;
        
        shared_ptr<LispVal> eval(shared_ptr<Environment>&) override {
            return shared_from_this();
        }

        bool operator==(const LispVal &val) const override{
            auto a = dynamic_cast<const Procedure*>(&val);
            return a && a->func == func;
        }

        shared_ptr<LispVal> call(const vector<shared_ptr<LispVal>> &param) {
            return func(param);
        }

        Procedure(const subr &func) : func(func) {}
    };

    struct LambdaProcedure : LispVal, std::enable_shared_from_this<LambdaProcedure>
    {
        vector<string> formals;
        shared_ptr<Pair> body;
        shared_ptr<Environment> environment;
        
        string show() const override;
        
        shared_ptr<LispVal> eval(shared_ptr<Environment>&) override {
            return shared_from_this();
        }

        bool operator==(const LispVal &) const override{
            throw not_implemented();
        }

        LambdaProcedure(const vector<string> &formals,
                        const shared_ptr<Pair> &body,
                        const shared_ptr<Environment> &environment) :
            formals(formals), body(body), environment(environment) {}
    };
}
