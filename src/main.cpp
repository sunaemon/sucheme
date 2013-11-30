#include <readline/readline.h>
#include <readline/history.h>
#include "parser.hpp"
#include "eval.hpp"
#include "functions.hpp"
#include "gc.hpp"

using namespace std;
using namespace sucheme;

int main(int, char**)
{
    char *command;

    using_history();
    read_history(".my_history");

    init_gc();

    Environment *e = alloc<Environment>(nullptr);
    init_environment(e);

    while((command = readline(">> "))) {
        add_history(command);

        if(!(setjmp(ex_jbuf))) {
            char *buf = show(eval(parse(command),e));
            puts(buf);
            free(buf);
        } else {
            fprintf(stderr, "%s\n", ex_buf);
        }
        run_gc(e);

        free(command);
    }
    write_history(".my_history");
}
