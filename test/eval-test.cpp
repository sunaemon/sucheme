#include "gtest/gtest.h"

#include <random>
#include <climits>
#include <iostream>
#include <string>
#include <memory>
#include "parser.h"
#include "environment.h"
#include "functions.h"
#include "list.h"
#include "eval.h"
#include "show.h"
#include "gc.h"
#include "intern.h"

using namespace std;

class EvalTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        e = alloc_Environment(nullptr);
        init_environment(e);
    }
public:
    Environment *e;
};

#define EVAL_TEST(a,b) EXPECT_EQ(string(show(parse(a))), string(show(eval(parse(b),e))));

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
    run_gc(&e);
    eval(parse("(define f (lambda (x) (cond ((= 0 x) 1)(#t (* (f (- x 1)) x)))))"), e);
    EVAL_TEST("3628800", "(f 10)");
    eval(parse("(define f (lambda (x) (cond ((= 0 x) 1) ((= 1 x) 1) (#t (+ (f (- x 1)) (f (- x 2)))))))"), e);
    EVAL_TEST("10946", "(f 20)");
    EVAL_TEST("121393", "(f 25)");
    //EVAL_TEST("121393", "(f 30)");
    run_gc(&e);
}

TEST_F(EvalTest, functions)
{
    EVAL_TEST("a", "(car (quote (a b)))");
    EVAL_TEST("(a b)", "(cadr (quote (c (a b))))");
    EVAL_TEST("#t", "(null? (quote ()))");
    EVAL_TEST("#f", "(null? (quote (a)))");
}
