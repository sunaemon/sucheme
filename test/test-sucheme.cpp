#include "../src/parser.hpp"
#include "../src/environment.hpp"
#include "../src/functions.hpp"
#include <random>
#include <climits>
#include <iostream>
#include <string>
#include <sstream>

namespace sucheme
{
    using std::cerr;
    using std::string;
    using std::endl;
    using std::string;
    using std::ostringstream;
    using std::get;
    using std::move;
    using std::make_shared;
    using std::dynamic_pointer_cast;

    void assert_equal(const string &expect, const string &actual)
    {
        if(expect != actual)
            cerr << "Expected: " << expect << " Actual: " << actual << endl;
    }

    void assert_equal(const int &expect, const int &actual)
    {
        if(expect != actual)
            cerr << "Expected: " << expect << " Actual: " << actual << endl;
    }

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
        assert_equal(i, dynamic_cast<Number*>(get<0>(ret).get())->integer);
        assert_equal(s.length(), get<1>(ret));
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
        assert_equal(1,dynamic_pointer_cast<Number>(dat_as_pair->car)->integer);
        auto sc = dynamic_pointer_cast<Pair>(dat_as_pair->cdr);
        assert_equal(2,dynamic_pointer_cast<Number>(sc->car)->integer);
        dynamic_cast<Empty*>(sc->cdr.get());
    }


    void test_parse(const string &s, const string &t)
    {
        auto ret = PExpr(t);
        auto dat = move(get<0>(ret));
        assert_equal(s.c_str(), dat->show().c_str());
    }

    void test_parse(const string &s)
    {
        test_parse(s,s);
    }

    void test_parse()
    {
        test_parse("((+) test)");
        test_parse("((test) ((>lsifsefj1111)))");
        test_parse("(test)", "( test )");
        test_parse("((test))", "( (test ) \
            )");
        test_parse("((+) ())", "((+)())");
    }

    void test_eval(shared_ptr<LispVal> a, shared_ptr<LispVal> b)
    {
        Environment e;
        e.env_map["+"] = make_shared<Procedure>(add);
        
        assert_equal(a->eval(e)->show().c_str(), b->show().c_str());
    }

    void test_plus()
    {
        test_eval(move(parse("(+ 1 2)")), move(parse("3")));
        test_eval(parse("(+ 1 2 5)"), parse("8"));
        test_eval(parse("(+ (+ 1 4) (+ 1 4 5))"), parse("15"));
    }

}

int main(int,char**)
{
    sucheme::test_number_parser();
    sucheme::test_parse();
    sucheme::test_list_parser();
    sucheme::test_plus();
}
