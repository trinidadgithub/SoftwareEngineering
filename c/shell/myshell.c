// Standard libraries for I/O, process management, and file operations.
#include "myshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 256
#define MAX_ARGS 32

// Function to split input into command and arguments
int parse_input(char *input, char **args, char **input_file, char **output_file, int *append) {
    *input_file = NULL;
    *output_file = NULL;
    *append = 0;
    int argc = 0;

    // Trim trailing newline
    input[strcspn(input, "\n")] = '\0';

    // Split input by spaces
    char *token = strtok(input, " ");
    while (token && argc < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            if (token) *input_file = token;
            continue;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            if (token) *output_file = token;
            *append = 0;
            continue;
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " ");
            if (token) *output_file = token;
            *append = 1;
            continue;
        }
        args[argc++] = token;
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;  // Null-terminate the args array
    return argc;
}

// Function to execute built-in commands
int execute_builtin(char **args, int argc) {
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {
        char *dir = argc > 1 ? args[1] : getenv("HOME");
        if (chdir(dir) != 0) {
            perror("cd");
        }
        return 1;
    }
    return 0;  // Not a built-in command
}

// Function to execute external commands
void execute_command(char **args, char *input_file, char *output_file, int append) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {  // Child process
        // Handle input redirection
        if (input_file) {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("open input");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        // Handle output redirection
        if (output_file) {
            int fd = open(output_file, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
            if (fd < 0) {
                perror("open output");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        // Execute command
        execvp(args[0], args);
        perror("execvp");  // Only reached if execvp fails
        exit(1);
    } else {  // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

#ifndef TEST_MODE
int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char *input_file, *output_file;
    int append;

    while (1) {
        printf("myshell$ ");
        fflush(stdout);

        if (!fgets(input, MAX_INPUT, stdin)) {
            break;
        }

        int argc = parse_input(input, args, &input_file, &output_file, &append);
        if (argc == 0 || args[0] == NULL) {
            continue;
        }

        if (execute_builtin(args, argc)) {
            continue;
        }

        execute_command(args, input_file, output_file, append);
    }

    return 0;
}
#endif

