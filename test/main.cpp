#include <stdio.h>

#include "gtest/gtest.h"
#include "gc.hpp"

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  sucheme::init_gc();
  testing::InitGoogleTest(&argc, argv);
  auto ret = RUN_ALL_TESTS();
  sucheme::term_gc();
  return ret;
}
