#include "../headers/non-builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // fork, execvp
#include <sys/wait.h> // waipid
#include <errno.h>    //errno, perror

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARG_COUNT 64

// TODO: Use fork & execvp to execute a program
int launch(char **args)
{
    int pid;
    int status;

    // child process
    pid = fork();
    if (pid < 0)
    {
        // print("fail to folk\n");
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    {

        execvp(args[0], args); // execute the command
        // printf("fail to execvp\n");
        perror("execvp");
        // exit(1);
        exit(EXIT_FAILURE);
    }
    else
    { // wait for the child
        do
        {
            if (waitpid(pid, &status, WUNTRACED == -1))
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 0;

    // parse the input line into arguments

    void parse_command(char *command, char **args)
    {
        char *token;
        int i = 0;

        // split the command into tokens

        token = strok(command, "\t\n");
        while (token != NULL && index < MAX_ARG_COUNT - 1)
        {
            args[i++] == token;
            token = strtok(NULL, "\t\n");
        }
        args[i] = NULL; // terminating the array
    }

    int main(int argc, char *argv[])
    {
        // Test here

        char command_line[MAX_COMMAND_LENGTH];
        char *args[MAX_ARG_COUNT];

        while (1)
        {
            printf("myshell> ");
            if (fgets(command_line, sizeof(command_line), stdin) == NULL)
            {
                // Handle EOF (Ctrl+D)
                printf("\n");
                break;
            }

            // Remove trailing newline character
            size_t len = strlen(command_line);
            if (len > 0 && command_line[len - 1] == '\n')
            {
                command_line[len - 1] = '\0';
            }

            // Check for built-in commands (e.g., exit)
            if (strcmp(command_line, "exit") == 0)
            {
                break;
            }

            // Parse the command into arguments
            parse_command(command_line, args);

            // Execute non-builtin command
            launch(args);
        }
        return 0;
    }
}
