#include "gtest/gtest.h"

#include <random>
#include <iostream>
#include <string>
#include <memory>
#include "../src/gc.hpp"
#include <climits>

using namespace sucheme;
using std::string;
using std::shared_ptr;

using namespace sucheme;
using namespace std;

TEST(GC, Test1)
{
    init_gc();

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

