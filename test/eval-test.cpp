#include "gtest/gtest.h"

#include <random>
#include <climits>
#include <iostream>
#include <string>
#include <memory>
#include "parser.hpp"
#include "environment.hpp"
#include "functions.hpp"
#include "list.hpp"
#include "cps.hpp"
#include "eval.hpp"
#include "show.hpp"
#include "gc.hpp"

using namespace sucheme;
using namespace std;

class EvalTest : public ::testing::Test {
    
protected:
    virtual void SetUp() {
        e = alloc<Environment>(nullptr);
        env_define(e, "+", alloc<Procedure>(sucheme::add));
        env_define(e, "=", alloc<Procedure>(sucheme::eq));
        env_define(e, "*", alloc<Procedure>(sucheme::mul));
        env_define(e, "-", alloc<Procedure>(sucheme::sub));
        env_define(e, "car", alloc<Procedure>(sucheme::car));
        env_define(e, "cdr", alloc<Procedure>(sucheme::cdr));
        env_define(e, "print", alloc<Procedure>(sucheme::print));
        env_define(e, "null?", alloc<Procedure>(sucheme::null_is));
        env_define(e, "else", alloc<Bool>(true));
        
        eval(parse("(define cadr (lambda (x) (car (cdr x))))"),e);
        eval(parse("(define cdar (lambda (x) (cdr (car x))))"),e);
        eval(parse("(define caar (lambda (x) (car (car x))))"),e);
        eval(parse("(define cddr (lambda (x) (cdr (cdr x))))"),e);
        
    }
public:
    Environment *e;
};

#define EVAL_TEST(a,b) EXPECT_EQ(show(parse(a)), show(eval(parse(b),e)));

TEST_F(EvalTest, Plus)
{
    EVAL_TEST("3", "(+ 1 2)");
    EVAL_TEST("8", "(+ 1 2 5)");
    EVAL_TEST("15", "(+ (+ 1 4) (+ 1 4 5))");
}

TEST_F(EvalTest, Lambda)
{
    EVAL_TEST("4", "((lambda (x) (+ 1 x)) 3)");
}

TEST_F(EvalTest, Set)
{
    EVAL_TEST("3", "(begin (define a 1) (define b 1) (set! b 2) (+ a b))");
}


TEST_F(EvalTest, Cond)
{
    EVAL_TEST("1", "(cond (#t 1))");
    EVAL_TEST("2", "(cond (#f 1) (#t 2) (#f 1))");
    EVAL_TEST("4", "(cond (#f 1) (#f 2) (#t 4))");
}

TEST_F(EvalTest, Rec)
{
    eval(parse("(define f (lambda (x) (cond ((= 0 x) 1)(#t (* (f (- x 1)) x)))))"), e);
    EVAL_TEST("3628800", "(f 10)");
    eval(parse("(define f (lambda (x) (cond ((= 0 x) 1) ((= 1 x) 1) (#t (+ (f (- x 1)) (f (- x 2)))))))"), e);
    EVAL_TEST("10946", "(f 20)");
    EVAL_TEST("121393", "(f 25)");
}

TEST_F(EvalTest, functions)
{
    EVAL_TEST("a", "(car (quote (a b)))");
    EVAL_TEST("(a b)", "(cadr (quote (c (a b))))");
    EVAL_TEST("#t", "(null? (quote ()))");
    EVAL_TEST("#f", "(null? (quote (a)))");
}
