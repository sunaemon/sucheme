#include <readline/readline.h>
#include <readline/history.h>
#include "parser.hpp"
#include "eval.hpp"
#include "functions.hpp"

#include <iostream>

using namespace std;
using namespace sucheme;

int main(int, char**)
{
    char *command;

    using_history();
    read_history(".my_history");

    auto e = make_shared<Environment>(shared_ptr<Environment>(nullptr));
    e->env_map["+"] = make_shared<Procedure>(sucheme::add);
    e->env_map["="] = make_shared<Procedure>(sucheme::eq);
    e->env_map["-"] = make_shared<Procedure>(sucheme::sub);
    e->env_map["*"] = make_shared<Procedure>(sucheme::mul);
    e->env_map["car"] = make_shared<Procedure>(sucheme::car);
    e->env_map["cdr"] = make_shared<Procedure>(sucheme::cdr);
    e->env_map["else"] = make_shared<Bool>(true);
    e->env_map["print"] = make_shared<Procedure>(sucheme::print);
    e->env_map["null?"] = make_shared<Procedure>(sucheme::null_is);

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
