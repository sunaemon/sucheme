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
        
        try {
            char *buf = show(eval(parse(command),e));
            puts(buf);
            free(buf);
        } catch (std::exception &e) {
            fprintf(stderr, "%s", e.what());
        }
        run_gc(e);

        free(command);
    }
    write_history(".my_history");
}
