#include <stdio.h>
#include <stdlib.h>

char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **split_line(char *line) {
    // TODO: Split the line into arguments
    return 0;
}
