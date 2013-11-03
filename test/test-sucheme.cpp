#include "../src/parser.hpp"
#include "../src/environment.hpp"
#include "../src/functions.hpp"
#include <random>
#include <climits>
#include <iostream>
#include <string>

namespace sucheme
{
    using std::cerr;
    using std::string;
    using std::endl;

    void assert_equal(const string &expect, const string &actual)
    {
        if(expect != actual)
            cerr << "Expected: " << expect << " Actual: " << actual << endl;
    }

    void test_eval(shared_ptr<LispVal> a, shared_ptr<LispVal> b)
    {
        Environment e;
        e.env_map["+"] = make_shared<Procedure>(add);
        
        assert_equal(a->eval(e)->show().c_str(), b->show().c_str());
    }

    void test_plus()
    {
        //test_eval(move(parse("(+ 1 2)")), move(parse("3")));
        parse("(+ 1 3)");
//        test_eval(*parse("(+ 1 2 5)"), *parse("8"));
//        test_eval(*parse("(+ (+ 1 4) (+ 1 4 5))"), *parse("15"));
    }

}

int main(int,char**)
{
    std::cerr << "test" << std::endl;

    sucheme::test_plus();
}
