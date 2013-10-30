#include <cppcutter.h>
#include "../src/parser.hpp"
#include <random>
#include <climits>
#include <iostream>

namespace sucheme
{
    using std::string;
    using std::ostringstream;
    using std::get;
    using std::move;

    string itos(int number)
    {
        std::ostringstream ss;
        ss << number;
        return ss.str();
    }
    
    void test_number_parser(int i)
    {
        string s = itos(i);
        auto ret = PExpr(s);
        cut_assert_equal_int(i, dynamic_cast<Number*>(get<0>(ret).get())->integer);
        cut_assert_equal_int(s.length(), get<1>(ret));
    }
    
    void test_number_parser()
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

    void test_list_parser()
    {
        auto ret = PExpr("(1 2)");
        auto dat = dynamic_cast<Pair*>(get<0>(ret).get());
        cut_assert_equal_int(1,dynamic_cast<Number*>(dat->car.get())->integer);
        auto sc = dynamic_cast<Pair*>(dat->cdr.get());
        cut_assert_equal_int(2,dynamic_cast<Number*>(sc->car.get())->integer);
        dynamic_cast<Empty*>(sc->cdr.get());
    }


    void test_parse(const string &s, const string &t)
    {
        auto ret = PExpr(t);
        auto dat = move(get<0>(ret));
        cut_assert_equal_string(s.c_str(), dat->show().c_str());
    }

    void test_parse(const string &s)
    {
        test_parse(s,s);
    }

    void test_parse()
    {
        try {
            test_parse("((+) test)");
            test_parse("((test) ((>lsifsefj1111)))");
            test_parse("(test)", "( test )");
            test_parse("((test))", "( (test ) \
            )");
            test_parse("((+) ())", "((+)())");
        } catch(std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void test_plus()
    {

    }
}
