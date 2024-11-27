#include "../headers/builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

// help prints a hello message and lists all the builtin functions with descriptions
int builtin_help(char **args) {
    printf("Shell Built-in Commands:\n");
    printf("help: Display this help message\n");
    printf("cd <directory>: Change the current directory\n");
    printf("ls: List files in the current directory\n");
    printf("exit: Exit the shell\n");
    return 1; // Returning 1 indicates the shell should continue running
}

// cd changes the current directory to the given directory
int builtin_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: Missing argument\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
    return 1;
}

// ls lists all the files in the current directory
int builtin_ls(char **args) {
    DIR *directory;
    struct dirent *entry;

    directory = opendir(".");
    if (directory == NULL) {
        perror("ls");
        return 1;
    }

    while ((entry = readdir(directory)) != NULL) {
        printf("%s  ", entry->d_name);
    }
    printf("\n");

    closedir(directory);
    return 1;
}

// exit exits the shell
int builtin_exit(char **args) {
    printf("Exiting shell...\n");
    return 0; // Returning 0 indicates the shell should terminate
}

int main(int argc, char *argv[]) {
    // Testing the built-in functions
    char *args_cd[] = {"cd", "test_directory", NULL};
    char *args_ls[] = {"ls", NULL};
    char *args_help[] = {"help", NULL};
    char *args_exit[] = {"exit", NULL};

    printf("Testing builtin_cd:\n");
    builtin_cd(args_cd);

    printf("\nTesting builtin_ls:\n");
    builtin_ls(args_ls);

    printf("\nTesting builtin_help:\n");
    builtin_help(args_help);

    printf("\nTesting builtin_exit:\n");
    if (builtin_exit(args_exit) == 0) {
        return 0;
    }

    return 0;
}
