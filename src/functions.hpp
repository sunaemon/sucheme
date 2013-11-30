#pragma once
#include "parser.hpp"
#include "environment.hpp"

void init_environment(Environment *e);
GCPtr add(unsigned int argc, const GCPtr argv[]);
GCPtr eq(unsigned int argc, const GCPtr argv[]);
GCPtr sub(unsigned int argc, const GCPtr argv[]);
GCPtr mul(unsigned int argc, const GCPtr argv[]);
GCPtr print(unsigned int argc, const GCPtr argv[]);
GCPtr car(unsigned int argc, const GCPtr argv[]);
GCPtr cdr(unsigned int argc, const GCPtr argv[]);
GCPtr null_is(unsigned int argc, const GCPtr argv[]);
