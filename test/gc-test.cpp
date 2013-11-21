#include "gtest/gtest.h"

#include <random>
#include <iostream>
#include <string>
#include <memory>
#include "gc.hpp"
#include <climits>
#include "functions.hpp"

using namespace sucheme;
using std::string;
using std::shared_ptr;

using namespace sucheme;
using namespace std;

TEST(GC, Test1)
{
    int *i = alloc<int>();
    int *j = alloc<int>();
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
}

TEST(GC, Test2)
{
    Environment *nullenv = nullptr;
    run_gc(nullenv);

    auto e = alloc<Environment>(nullptr);
    env_define(e, "+", alloc<Procedure>(sucheme::add));
    env_define(e, "=", alloc<Procedure>(sucheme::eq));
    env_define(e, "*", alloc<Procedure>(sucheme::mul));
    env_define(e, "-", alloc<Procedure>(sucheme::sub));
    env_define(e, "car", alloc<Procedure>(sucheme::car));
    env_define(e, "cdr", alloc<Procedure>(sucheme::cdr));
    env_define(e, "print", alloc<Procedure>(sucheme::print));
    env_define(e, "null?", alloc<Procedure>(sucheme::null_is));
    env_define(e, "else", alloc<Bool>(true));

    auto before=allocated_memory();
    
    run_gc(e);

    auto after=allocated_memory();

    EXPECT_EQ(before, after);
}
