#include <stdio.h>

#include "gtest/gtest.h"
#include "gc.h"
#include "exceptions.h"

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  init_gc();
  testing::InitGoogleTest(&argc, argv);
  int ret;
  if(!(setjmp(ex_jbuf))) {
      ret = RUN_ALL_TESTS();
  } else {
      fprintf(stderr, "get exception; terminate test. error:%s\n", ex_buf);
  }
  term_gc();
  return ret;
}
