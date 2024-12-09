#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "headers/read.h"
#include "headers/split.h"
#include "headers/execute.h"
#include <linux/limits.h>

// Function to execute commands based on operators like ;, &&, ||
int execute_commands(char **commands, int num_tokens)
{
    int status = 1;  // Start with successful status

    for (int i = 0; i < num_tokens; i++)
    {
        if (strcmp(commands[i], "&&") == 0)
        {
            if (status == 0)  // If the previous command failed, don't execute the next one
                break;
        }
        else if (strcmp(commands[i], "||") == 0)
        {
            if (status != 0)  // If the previous command succeeded, don't execute the next one
                break;
        }
        else if (strcmp(commands[i], ";") == 0)
        {
            // Continue to the next command without checking status
            continue;
        }
        else
        {
            // Execute the command normally
            status = execute(&commands[i]);
        }
    }

    return status;
}

int main()
{
    char *line;
    char **tokens;
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

        // Split the line based on space, semicolon, &&, and ||
        tokens = split_line(line, &num_tokens);

        // Execute the commands and manage status based on &&, ||, ;
        status = execute_commands(tokens, num_tokens);

        // Clean up
        free(line);
        free(tokens);
    }
    return 0;
}
