#include "../headers/non-builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // fork, execvp
#include <sys/wait.h> // waipid
#include <errno.h>    //errno, perror
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARG_COUNT 64

int execute_pipeline(char **args) {
    int i = 0;
    int pipe_count = 0;

    // Count pipes
    while (args[i] != NULL) {
        if (strcmp(args[i], "|") == 0) {
            pipe_count++;
        }
        i++;
    }

    if (pipe_count == 0) {
        return launch(args);
    }

    int cmd_count = pipe_count + 1;
    int pipes[pipe_count][2];
    char **cmd_args[cmd_count];
    int cmd_index = 0;
    int arg_index = 0;

    // Split commands at pipe symbols
    cmd_args[cmd_index] = malloc(sizeof(char*) * MAX_ARG_COUNT);
    i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "|") == 0) {
            cmd_args[cmd_index][arg_index] = NULL;
            cmd_index++;
            cmd_args[cmd_index] = malloc(sizeof(char*) * MAX_ARG_COUNT);
            arg_index = 0;
        } else {
            cmd_args[cmd_index][arg_index++] = args[i];
        }
        i++;
    }
    cmd_args[cmd_index][arg_index] = NULL;

    // Create pipes
    for (i = 0; i < pipe_count; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return 1;
        }
    }

    // Execute commands
    for (i = 0; i <= pipe_count; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // First command
            if (i == 0) {
                dup2(pipes[0][1], STDOUT_FILENO);
            }
            // Last command
            else if (i == pipe_count) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            // Middle commands
            else {
                dup2(pipes[i-1][0], STDIN_FILENO);
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe fds
            for (int j = 0; j < pipe_count; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            launch(cmd_args[i]);
            exit(EXIT_FAILURE);
        }
    }
    // Parent closes all pipe fds
    for (i = 0; i < pipe_count; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (i = 0; i <= pipe_count; i++) {
        wait(NULL);
    }

    // Free allocated memory
    for (i = 0; i <= pipe_count; i++) {
        free(cmd_args[i]);
    }

    return 1;
}

int launch(char **args)
{
    int pid;
    int status;
    int background = 0;
    int signals[] = {SIGSEGV, SIGFPE, SIGILL, SIGBUS, SIGXCPU, SIGXFSZ, SIGPROF, SIGSYS};
    int signal_count = sizeof(signals) / sizeof(signals[0]);
    char *signal_names[] = {"segmentation fault", "floating point exception", "illegal instruction", "bus error", "CPU time limit exceeded", "file size limit exceeded", "real-time limit exceeded", "bad system call"};

    // Check if last argument is &
    for (int i = 0; args[i] != NULL; i++)
    {
        if (args[i + 1] == NULL && strcmp(args[i], "&") == 0)
        {
            background = 1;
            args[i] = NULL; // Remove the & from arguments
            break;
        }
    }

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
        if (!background)
        {
            do
            {
                if (waitpid(pid, &status, WUNTRACED) == -1)
                {
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));

            if (WIFSIGNALED(status))
            {
                for (int i = 0; i < signal_count; i++)
                {
                    if (WTERMSIG(status) == signals[i])
                    {
                        printf("Terminated by %s\n", signal_names[i]);
                        break;
                    }
                    if (i == signal_count - 1)
                    {
                        printf("Terminated by unknown signal\n");
                    }
                }
            }
        }
        else
        {
            printf("[%d] started in background\n", pid);
        }
    }
    return 1;
}

// parse the input line into arguments
void parse_command(char *command, char **args)
{
    char *token;
    int i = 0;

    // split the command into tokens
    token = strtok(command, " \t\n");
    while (token != NULL && i < MAX_ARG_COUNT - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL; // terminating the array
}

int main()
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

        char *cmd = command_line;
        char *start = cmd;
        int parse_len = 0;

        // Skip leading whitespace
        while (*start == ' ' || *start == '\t')
            start++;
        cmd = start;

        while (*cmd != '\0')
        {
            if (*cmd == '&')
            {
                // Get length excluding trailing whitespace
                char *end = cmd - 1;
                while (end > start && (*end == ' ' || *end == '\t'))
                    end--;
                parse_len = end - start + 1;

                // Copy command including & if not at end
                char command[1024];
                strncpy(command, start, parse_len);
                command[parse_len] = '\0';

                // Add & if not the last command
                if (*(cmd + 1) != '\0')
                {
                    strcat(command, " &");
                }

                parse_command(command, args);
                launch(args);

                // Move to start of next command
                start = cmd + 1;
                while (*start == ' ' || *start == '\t')
                    start++;
            }
            cmd++;
        }

        // Handle last command if it exists
        if (*start != '\0')
        {
            // Remove trailing whitespace
            char *end = cmd - 1;
            while (end > start && (*end == ' ' || *end == '\t'))
                end--;
            parse_len = end - start + 1;

            char command[1024]; // Using fixed size instead of undefined MAX_COMMAND_LENGTH
            strncpy(command, start, parse_len);
            command[parse_len] = '\0';
            parse_command(command, args);
            launch(args);
        }
    }
}
