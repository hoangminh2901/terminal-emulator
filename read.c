#include <stdio.h>
#include <stdlib.h>
#include "headers/read.h"

char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}