#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "headers/read.h"
#include "headers/split.h"
#include "headers/execute.h"

int main(int argc, char *argv[])
{
    char *line;
    char **args;
    int num_tokens;
    int status;
    char *cwd = getcwd(NULL, 0);

    do
    {
        printf("%s>> ", cwd);
        line = read_line();
        args = split_line(line, &num_tokens);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
    return 0;
}