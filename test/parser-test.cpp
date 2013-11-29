#include "gtest/gtest.h"

#include <random>
#include <climits>
#include <iostream>
#include <string>
#include <memory>
#include "parser.hpp"
#include "environment.hpp"
#include "functions.hpp"
#include "list.hpp"
#include "show.hpp"

using namespace sucheme;
using std::string;
using std::shared_ptr;

using namespace sucheme;
using namespace std;

void test_number_parser(int i)
{
    string s = to_string(i);
    auto ret = PExpr(s.c_str());
    EXPECT_EQ(i, dcast<Number>(ret.val)->integer);
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

    GCObject *dat = ret.val;
    auto dat_as_pair = dcast<Pair>(dat);
    EXPECT_EQ(1,dcast<Number>(dat_as_pair->car)->integer);
    auto sc = dcast<Pair>(dat_as_pair->cdr);
    EXPECT_EQ(2,dcast<Number>(sc->car)->integer);
    dcast_ex<Empty>(sc->cdr);
}

TEST(List, make_list)
{
    EXPECT_EQ(show(make_list(parse("2"), parse("(3 3)"))), "(2 (3 3))");
}

void test_parse(const char *s, const char *t)
{
    auto ret = PExpr(t);
    auto dat = move(ret.val);
    EXPECT_EQ(s, show(dat));
}

void test_parse(const char *s)
{
    test_parse(s,s);
}

TEST(Parser, Parse0)
{
    test_parse("((+) test #t #f)");
}
TEST(Parser, Parse1)
{
    test_parse("((+) test)");
}
TEST(Parser, Parse2)
{
    test_parse("((test) ((>lsifsefj1111)))");
}
TEST(Parser, Parse3)
{
    test_parse("(test)", "( test )");
}
TEST(Parser, Parse4)
{
    test_parse("((test))", "( (test ) \
    )");
}
TEST(Parser, Parse5)
{
    test_parse("((+) ())", "((+)())");
}
TEST(Parser, Parse6)
{
    test_parse("((lambda (x) (+ 1 x)) 3)");
}
TEST(Parser, Parse7)
{
    EXPECT_EQ(true, initial('!'));
}
TEST(Parser, Parse8)
{
    EXPECT_EQ(true, initial('~'));
}
TEST(Parser, Parse9)
{
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

void test_cps(shared_ptr<GCObject> a, shared_ptr<GCObject> b, shared_ptr<GCObject> cont)
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
