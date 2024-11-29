#include <stdio.h>
#include <stdlib.h>
#include "headers/read.h"
#include <unistd.h>

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t chars = getline(&line, &bufsize, stdin);
    if (bufsize > 0 && line[chars - 1] == '\n')
    {
        line[chars - 1] = '\0';
        --chars;
    }
    return line;
}

int main(int argc, char *argv[])
{
    char *line;
    while (1)
    {
        printf("$ ");
        line = read_line();
        printf("%s\n", line);
        free(line);
    }
    return 0;
}