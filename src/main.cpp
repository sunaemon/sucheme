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

    init_gc();

    auto e = alloc<Environment>(nullptr);
    init_environment(e);

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
