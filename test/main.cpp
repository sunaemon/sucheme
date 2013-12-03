#include <stdio.h>

#include "gtest/gtest.h"
#include "gc.h"

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  init_gc();
  testing::InitGoogleTest(&argc, argv);
  auto ret = RUN_ALL_TESTS();
  term_gc();
  return ret;
}
