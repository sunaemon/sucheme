#include "gtest/gtest.h"

#include <random>
#include <climits>
#include <iostream>
#include <string>
#include <memory>
#include "../src/parser.hpp"
#include "../src/environment.hpp"
#include "../src/functions.hpp"
#include "../src/list.hpp"
#include "../src/cps.hpp"
#include "../src/show.hpp"

using namespace sucheme;
using std::string;
using std::shared_ptr;

using namespace sucheme;
using namespace std;

void test_number_parser(int i)
{
    string s = to_string(i);
    auto ret = PExpr(s);
    EXPECT_EQ(i, dynamic_cast<Number*>(ret.val.get())->integer);
    EXPECT_EQ((int)s.length(), ret.pos);
}

TEST(Parser, Number)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    
    for(int i=-10; i<10; ++i) {
        test_number_parser(i);
        test_number_parser(mt());
    }
    test_number_parser(INT_MIN);
    test_number_parser(INT_MAX);
}

class test : public std::enable_shared_from_this<test> {};

TEST(Parser, List)
{
    auto ret = PExpr("(1 2)");

    shared_ptr<LispVal> dat = std::move(ret.val);
    auto dat_as_pair = dynamic_pointer_cast<Pair>(dat);
    EXPECT_EQ(1,dynamic_pointer_cast<Number>(dat_as_pair->car)->integer);
    auto sc = dynamic_pointer_cast<Pair>(dat_as_pair->cdr);
    EXPECT_EQ(2,dynamic_pointer_cast<Number>(sc->car)->integer);
    dynamic_cast<Empty*>(sc->cdr.get());
}

TEST(List, make_list)
{
    EXPECT_EQ(show(make_list(parse("2"), parse("(3 3)"))), "(2 (3 3))");
}

void test_parse(const string &s, const string &t)
{
    auto ret = PExpr(t);
    auto dat = move(ret.val);
    EXPECT_EQ(s, show(dat));
}

void test_parse(const string &s)
{
    test_parse(s,s);
}

TEST(Parser, Parse1)
{
    test_parse("((+) test #t #f)");
    test_parse("((+) test)");
    test_parse("((test) ((>lsifsefj1111)))");
    test_parse("(test)", "( test )");
    test_parse("((test))", "( (test ) \
    )");
    test_parse("((+) ())", "((+)())");
    test_parse("((lambda (x) (+ 1 x)) 3)");
    EXPECT_EQ(true, initial('!'));
    EXPECT_EQ(true, initial('~'));
    test_parse("(begin (set! a 1) (set! b 2) (+ a b))");
}

/*
TEST(Cps, IsSimple)
{
    auto e = make_shared<Environment>(shared_ptr<Environment>(nullptr));
    e->env_map["+"] = make_shared<Procedure>(sucheme::add);
    e->env_map["="] = make_shared<Procedure>(sucheme::eq);
    e->env_map["-"] = make_shared<Procedure>(sucheme::sub);
    e->env_map["*"] = make_shared<Procedure>(sucheme::mul);
    e->env_map["else"] = make_shared<Bool>(true);
    e->env_map["print"] = make_shared<Procedure>(sucheme::print);

    EXPECT_EQ(true, have(e.get(), "+"));
    EXPECT_EQ(false, (bool)have(e.get(), "f"));
    EXPECT_EQ(true, is_simple(parse("+"), *e));
    EXPECT_EQ(true, is_simple(parse("3"), *e));
    EXPECT_EQ(false, (bool)is_simple(parse("f"), *e));
    EXPECT_EQ(true, is_simple(parse("(+ 1 3)"), *e));
}

void test_cps(shared_ptr<LispVal> a, shared_ptr<LispVal> b, shared_ptr<LispVal> cont)
{
    auto e = make_shared<Environment>(shared_ptr<Environment>(nullptr));
    e->env_map["+"] = make_shared<Procedure>(sucheme::add);
    e->env_map["="] = make_shared<Procedure>(sucheme::eq);
    e->env_map["-"] = make_shared<Procedure>(sucheme::sub);
    e->env_map["*"] = make_shared<Procedure>(sucheme::mul);
    e->env_map["else"] = make_shared<Bool>(true);
    e->env_map["print"] = make_shared<Procedure>(sucheme::print);

    EXPECT_EQ(show(a),show(cps(b, cont, *e)));
}


TEST(Cps, simple)
{
    test_cps(parse("(C f)"), parse("f"), parse("C"));

    //cerr << show(cps(parse("(letrec ((f (lambda (x) (cond ((= 0 x) 1)(#t (* (f (- x 1)) x))))))(f 2))"), parse("C"), *e)) << endl;
//    cerr << cps(parse("(letrec ((f (lambda (x) (cond ((= 0 x) 1)(#t (* (f (- x 1)) x))))))(f 2))"), parse("print"),*e)->eval(e)->show()<< endl;
}

*/
