#include <gtest/gtest.h>

#include <iostream>

#include "parser.h"
#include "environment.h"
#include "gc.h"
#include "intern.h"

using namespace std;

stringstream ost;

static void callback(int id, const GCPtr l) {
    ost << extern_symbol(id) << " = " << show(l) << endl;
}

TEST(Environment_test, test1)
{
    auto e = alloc_Environment(nullptr);
    env_intern_define(e, "d", parse("1"));
    env_intern_define(e, "d", parse("2"));
    env_intern_define(e, "f", parse("3"));
    env_intern_define(e, "c", parse("4"));
    env_intern_define(e, "g", parse("5"));
    env_intern_define(e, "b", parse("6"));
    env_intern_define(e, "h", parse("7"));
    env_intern_define(e, "a", parse("8"));
    env_intern_define(e, "i", parse("9"));
    
    env_foreach(callback,e);
    EXPECT_EQ("a = 8\nb = 6\nd = 2\nf = 3\nc = 4\ng = 5\nh = 7\ni = 9\n", ost.str());
    //EXPECT_EQ("a = 8\nb = 6\nc = 4\nd = 2\nf = 3\ng = 5\nh = 7\ni = 9\n",ost.str());
}
