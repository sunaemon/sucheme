#include <random>
#include <climits>
#include <iostream>
#include <sstream>
#include "parser-test.hpp"

using namespace sucheme;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( parser_test );

string itos(int number)
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

void parser_test::test_number_parser(int i)
{
    string s = itos(i);
    auto ret = PExpr(s);
    CPPUNIT_ASSERT_EQUAL(i, dynamic_cast<Number*>(get<0>(ret).get())->integer);
    CPPUNIT_ASSERT_EQUAL((int)s.length(), get<1>(ret));
}
    
void parser_test::test_number_parser()
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

void parser_test::test_list_parser()
{
    auto ret = PExpr("(1 2)");
    shared_ptr<LispVal> dat = std::move(get<0>(ret));
    auto dat_as_pair = dynamic_pointer_cast<Pair>(dat);
    CPPUNIT_ASSERT_EQUAL(1,dynamic_pointer_cast<Number>(dat_as_pair->car)->integer);
    auto sc = dynamic_pointer_cast<Pair>(dat_as_pair->cdr);
    CPPUNIT_ASSERT_EQUAL(2,dynamic_pointer_cast<Number>(sc->car)->integer);
    dynamic_cast<Empty*>(sc->cdr.get());
}


void parser_test::test_parse(const string &s, const string &t)
{
    auto ret = PExpr(t);
    auto dat = move(get<0>(ret));
    CPPUNIT_ASSERT_EQUAL(s.c_str(), dat->show().c_str());
}

void parser_test::test_parse(const string &s)
{
    test_parse(s,s);
}

void parser_test::test_parse()
{
    test_parse("((+) test)");
    test_parse("((test) ((>lsifsefj1111)))");
    test_parse("(test)", "( test )");
    test_parse("((test))", "( (test ) \
            )");
    test_parse("((+) ())", "((+)())");
}

void parser_test::test_eval(shared_ptr<LispVal> a, shared_ptr<LispVal> b)
{
    Environment e;
    e.env_map["+"] = make_shared<Procedure>(add);
        
    CPPUNIT_ASSERT_EQUAL(a->eval(e)->show().c_str(),b->show().c_str());
}

void parser_test::test_plus()
{
    test_eval(move(parse("(+ 1 2)")), move(parse("3")));
    test_eval(parse("(+ 1 2 5)"), parse("8"));
    test_eval(parse("(+ (+ 1 4) (+ 1 4 5))"), parse("15"));
}
