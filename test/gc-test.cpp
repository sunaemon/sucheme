#include "gtest/gtest.h"

#include <random>
#include <iostream>
#include <string>
#include <memory>
#include "gc.h"
#include <climits>
#include "functions.h"
#include "parser.h"
#include "eval.h"

using namespace std;

using std::string;

/*TEST(GC, Test1)
{
    int *i = alloc_int();
    int *j = alloc_int();
    *i = 0;
    *j = 0;
    EXPECT_EQ(0, *i);
    EXPECT_EQ(0, *j);

    *i = INT_MAX;
    EXPECT_EQ(INT_MAX, *i);
    EXPECT_EQ(0, *j);

    *i = 0;
    EXPECT_EQ(0, *i);
    EXPECT_EQ(0, *j);

    *j = INT_MAX;
    EXPECT_EQ(0, *i);
    EXPECT_EQ(INT_MAX, *j);

    *j = 0;
    EXPECT_EQ(0, *i);
    EXPECT_EQ(0, *j);
    }*/


TEST(GC, Test2)
{
    Environment *nullenv = nullptr;
    run_gc(&nullenv);

    auto e = alloc_Environment(nullptr);
    env_intern_define(e, "car", ucast(alloc_Procedure(fun_car)));

    eval(parse("(define caar (lambda (x) (car (car x))))"),e);

    run_gc(&e);

    memset(mem[1-memory_in_used], 0, memsize);

    EXPECT_EQ("a", string(show(eval(parse("(caar (quote ((a b) a b c)))"),e))));
}
