#include <cppcutter.h>
#include "parser.hpp"


namespace test
{

void test_parser()
{
    auto a = PNumber("10");

    auto b = std::get<0>(a);

    cut_assert_equal_int(10, nv::get<NumberData>(b).integer);
}

}
