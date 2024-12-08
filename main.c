#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "headers/read.h"
#include "headers/split.h"
#include "headers/execute.h"
#include <linux/limits.h>

int main()
{
    char *line;
    char **args;
    int num_tokens;
    int status = 1;
    char cwd[PATH_MAX];

    while (status)
    {
        getcwd(cwd, sizeof(cwd));
        printf("%s>> ", cwd);
        line = read_line();
        if (line == NULL || line[0] == '\0')
        {
            continue;
        }
        args = split_line(line, &num_tokens);
        status = execute(args);

        free(line);
        free(args);
    }
    return 0;
}