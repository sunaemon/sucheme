#include <cppcutter.h>
#include "parser.hpp"
#include <random>

namespace test
{

    std::string itos(int number)
    {
        std::ostringstream ss;
        ss << number;
        return ss.str();
    }
    
    void test_number_parser(int i)
    {
        std::string s = itos(i);
        auto ret = PExpr(s);
        cut_assert(nv::type(std::get<0>(ret)) == typeid(NumberData));
        cut_assert_equal_int(i, nv::get<NumberData>(std::get<0>(ret)).integer);
        cut_assert_equal_int(s.length(), std::get<1>(ret));
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

/*    void test_list_parser()
    {
        auto ret = PExpr("(1 2)");
        auto dat = std::get<0>(ret);
        cut_assert(nv::type(dat) == typeid(ListData));
        std::list<LispVal> data = nv::get<ListData>(dat);
        cut_assert_equal_int(2, data.size());
        auto it = data.begin();
        cut_assert_equal_int(1, nv::get<NumberData>(*it++).integer);
        cut_assert_equal_int(2, nv::get<NumberData>(*it++).integer);
        //cut_assert_equal_int(s.length(), std::get<1>(ret));
    }

    void test_identifier()
    {
        auto ret = PExpr("(test test)");
        auto dat = std::get<0>(ret);
        cut_assert(nv::type(dat) == typeid(ListData));
        std::list<LispVal> data = nv::get<ListData>(dat);
        cut_assert_equal_int(2, data.size());
        auto it = data.begin();
        cut_assert_equal_string("test", nv::get<SymbolData>(*it++).c_str());
        cut_assert_equal_string("test", nv::get<SymbolData>(*it++).c_str());
        //cut_assert_equal_int(s.length(), std::get<1>(ret));
    }

    void test_identifier2()
    {
        auto ret = PExpr("(+ test)");
        auto dat = std::get<0>(ret);
        cut_assert(nv::type(dat) == typeid(ListData));
        std::list<LispVal> data = nv::get<ListData>(dat);
        cut_assert_equal_int(2, data.size());
        auto it = data.begin();
        cut_assert_equal_string("+", nv::get<SymbolData>(*it++).c_str());
        cut_assert_equal_string("test", nv::get<SymbolData>(*it++).c_str());

        //cut_assert_equal_int(s.length(), std::get<1>(ret));
    }

    void test_parse(const std::string &s)
    {
        auto ret = PExpr(s);
        auto dat = std::get<0>(ret);
        cut_assert_equal_string(s.c_str(), show(dat).c_str());
    }

    void test_parse(const std::string &s, const std::string &t)
    {
        auto ret = PExpr(t);
        auto dat = std::get<0>(ret);
        cut_assert_equal_string(s.c_str(), show(dat).c_str());
    }

    void test_parse()
    {
        test_parse("((+) test)");
        test_parse("((test) ((>lsifsefj1111)))");
        test_parse("(test)", "( test )");
        test_parse("((test))", "( (test )\
 )");
        test_parse("((+) ())", "((+)())");

    }

    void test_plus()
    {

    }*/
}
