#include <readline/readline.h>
#include <readline/history.h>
#include "parser.hpp"
#include "eval.hpp"
#include "functions.hpp"
#include "gc.hpp"
#include <iostream>

using namespace std;
using namespace sucheme;

int main(int, char**)
{
    char *command;

    using_history();
    read_history(".my_history");

    auto e = alloc<Environment>(nullptr);
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

    while((command = readline(">> "))) {
        add_history(command);
        
        try {
            cout << show(eval(parse(command),e)) << endl;
        } catch (std::exception &e) {
            cerr << e.what() << endl;
        }

        free(command);
    }
    write_history(".my_history");
}
