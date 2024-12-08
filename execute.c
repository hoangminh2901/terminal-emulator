#include "headers/builtin.h"
#include "headers/non-builtin.h"
#include "headers/execute.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>



// Check if the command is a builtin function
// If it is, execute it
// If it is not, execute it as a non-builtin function
int execute(char **args)
{
    if (args[0] == NULL)
    {
        return 1;
    }

    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "|") == 0) {
            return execute_pipeline(args);
        }
        i++;
    }

    char *builtin_str[] = {
        "help",
        "cd",
        "ls",
        "exit"};

    int (*builtin_func[])(char **) = {
        &builtin_help,
        &builtin_cd,
        &builtin_ls,
        &builtin_exit};

    int num_builtins = sizeof(builtin_str) / sizeof(char *);
    for (int i = 0; i < num_builtins; i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }
    return launch(args);
}