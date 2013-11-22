#include "gtest/gtest.h"
#include "intern.cpp"

using namespace sucheme;

TEST(Intern, test1)
{
    EXPECT_EQ(false, intern("abd")==intern("aaa"));
    EXPECT_EQ(intern("abd"), intern("abd"));
}
