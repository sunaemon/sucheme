#include <cppcutter.h>
#include "../src/parser.hpp"
#include "../src/environment.hpp"
#include "../src/functions.hpp"
#include <random>
#include <climits>
#include <iostream>
#include <sstream>

namespace sucheme
{
    using std::string;
    using std::ostringstream;
    using std::get;
    using std::move;
    using std::make_shared;
    using std::dynamic_pointer_cast;

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
        shared_ptr<LispVal> dat = std::move(get<0>(ret));
        auto dat_as_pair = dynamic_pointer_cast<Pair>(dat);
        cut_assert_equal_int(1,dynamic_pointer_cast<Number>(dat_as_pair->car)->integer);
        auto sc = dynamic_pointer_cast<Pair>(dat_as_pair->cdr);
        cut_assert_equal_int(2,dynamic_pointer_cast<Number>(sc->car)->integer);
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

    void test_eval(shared_ptr<LispVal> a, shared_ptr<LispVal> b)
    {
        Environment e;
        e.env_map["+"] = make_shared<Procedure>(add);
        
        cut_assert_equal_string(a->eval(e)->show().c_str(),b->show().c_str());
    }

    void test_plus()
    {
        test_eval(move(parse("(+ 1 2)")), move(parse("3")));
//        test_eval(*parse("(+ 1 2 5)"), *parse("8"));
//        test_eval(*parse("(+ (+ 1 4) (+ 1 4 5))"), *parse("15"));
    }
}
