#include <readline/readline.h>
#include <readline/history.h>
#include "parser.hpp"

int main(int, char**)
{
    char *command;

    using_history();
    read_history(".my_history");
    while((command = readline(">> "))) {
        add_history(command);

        printf("%s\n", command);

        free(command);
    }
    write_history(".my_history");
}

