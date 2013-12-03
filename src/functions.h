#pragma once
#include "parser.h"
#include "environment.h"
#include "macro.h"

IF_CPP(extern "C" {)

void init_environment(Environment *e);
GCPtr fun_add(unsigned int argc, const GCPtr argv[]);
GCPtr fun_eq(unsigned int argc, const GCPtr argv[]);
GCPtr fun_sub(unsigned int argc, const GCPtr argv[]);
GCPtr fun_mul(unsigned int argc, const GCPtr argv[]);
GCPtr fun_print(unsigned int argc, const GCPtr argv[]);
GCPtr fun_car(unsigned int argc, const GCPtr argv[]);
GCPtr fun_cdr(unsigned int argc, const GCPtr argv[]);
GCPtr fun_null_is(unsigned int argc, const GCPtr argv[]);

IF_CPP(})
