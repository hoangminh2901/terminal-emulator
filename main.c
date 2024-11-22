#include <stddef.h>
#include "read&split.c"
#include "headers/execute.h"

int main(int argc, char *argv[])
{
    char *line;
    char **args;
    int status;

    // TODO: Show directory
    do
    {
        printf(">> ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
    return 0;
}