#include "gtest/gtest.h"

#include <iostream>

#include "parser.hpp"
#include "environment.hpp"
#include "gc.hpp"
#include "intern.hpp"

using namespace sucheme;
using namespace std;

TEST(Environment_test, test1)
{
    auto e = alloc<Environment>(nullptr);
    env_intern_define(e, "d", parse("1"));
    env_intern_define(e, "d", parse("2"));
    env_intern_define(e, "f", parse("3"));
    env_intern_define(e, "c", parse("4"));
    env_intern_define(e, "g", parse("5"));
    env_intern_define(e, "b", parse("6"));
    env_intern_define(e, "h", parse("7"));
    env_intern_define(e, "a", parse("8"));
    env_intern_define(e, "i", parse("9"));
    stringstream ost;
    env_foreach([&](int id, const GCPtr l){
            ost << extern_symbol(id) << " = " << show(l) << endl;
        }, e);
    EXPECT_EQ("a = 8\nb = 6\nd = 2\nf = 3\nc = 4\ng = 5\nh = 7\ni = 9\n", ost.str());
    //EXPECT_EQ("a = 8\nb = 6\nc = 4\nd = 2\nf = 3\ng = 5\nh = 7\ni = 9\n",ost.str());
}
