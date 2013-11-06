#include "gtest/gtest.h"

#include <random>
#include <climits>
#include <iostream>
#include <string>
#include <memory>
#include "../src/parser.hpp"
#include "../src/environment.hpp"
#include "../src/functions.hpp"

using namespace sucheme;
using std::string;
using std::shared_ptr;

using namespace sucheme;
using namespace std;

void test_number_parser(int i)
{
    string s = to_string(i);
    auto ret = PExpr(s);
    EXPECT_EQ(i, dynamic_cast<Number*>(get<0>(ret).get())->integer);
    EXPECT_EQ((int)s.length(), get<1>(ret));
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

TEST(Parser, List)
{
    auto ret = PExpr("(1 2)");
    shared_ptr<LispVal> dat = std::move(get<0>(ret));
    auto dat_as_pair = dynamic_pointer_cast<Pair>(dat);
    EXPECT_EQ(1,dynamic_pointer_cast<Number>(dat_as_pair->car)->integer);
    auto sc = dynamic_pointer_cast<Pair>(dat_as_pair->cdr);
    EXPECT_EQ(2,dynamic_pointer_cast<Number>(sc->car)->integer);
    dynamic_cast<Empty*>(sc->cdr.get());
}


void test_parse(const string &s, const string &t)
{
    auto ret = PExpr(t);
    auto dat = move(get<0>(ret));
    EXPECT_EQ(s, dat->show());
}

void test_parse(const string &s)
{
    test_parse(s,s);
}

TEST(Parser, Parse)
{
    test_parse("((+) test #b #f)");
    test_parse("((+) test)");
    test_parse("((test) ((>lsifsefj1111)))");
    test_parse("(test)", "( test )");
    test_parse("((test))", "( (test ) \
    )");
    test_parse("((+) ())", "((+)())");
    test_parse("((lambda (x) (+ 1 x)) 3)");
}

void test_eval(shared_ptr<LispVal> a, shared_ptr<LispVal> b)
{
    auto e = make_shared<Environment>();
    e->env_map["+"] = make_shared<Procedure>(add);
        
    EXPECT_EQ(a->eval(e)->show(),b->show());
}

TEST(Eval, Plus)
{
    test_eval(parse("(+ 1 2)"), parse("3"));
    test_eval(parse("(+ 1 2 5)"), parse("8"));
    test_eval(parse("(+ (+ 1 4) (+ 1 4 5))"), parse("15"));
}

TEST(Eval, Lambda)
{
    test_eval(parse("((lambda (x) (+ 1 x)) 3)"), parse("4"));
}
