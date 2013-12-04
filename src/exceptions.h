#pragma once
#include <setjmp.h>
#include <string.h>
#include <stdio.h>
#include "macro.h"

IF_CPP(extern "C" {)

extern char ex_buf[];
extern jmp_buf ex_jbuf;
#define throw_jump()                                                 \
  {                                                                             \
      sprintf(ex_buf+strlen(ex_buf), " (in %s on %d)", __FILE__, __LINE__); \
      longjmp(ex_jbuf, __LINE__);                                       \
  }

#define throw_jumpf(...)                                                 \
  {                                                                             \
      sprintf(ex_buf, __VA_ARGS__);                             \
      sprintf(ex_buf+strlen(ex_buf), " (in %s on %d)", __FILE__, __LINE__); \
      longjmp(ex_jbuf, __LINE__);                                       \
  }

IF_CPP(})

