#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  // for access(), fork(), execvp(), chdir()
#include <sys/wait.h> // for waitpid()
#include <limits.h>  // for PATH_MAX
#include <errno.h>   // for errno

#define PATH_MAX_LENGTH 4096
#define MAX_ARGS 100

// Function declarations
static int is_builtin(const char *command);
char *find_in_path(const char *cmd, const char *path_env);
int tokenize_command(const char *cmd, char *args[]);
void handle_quotes(char *input, char *args[], int *argc);


int main() {
    setbuf(stdout, NULL);  // Initial flush to ensure output is unbuffered

    char input[100];
    char command[PATH_MAX_LENGTH];
    char *path = getenv("PATH");
    char *args[MAX_ARGS];
    char home_dir[PATH_MAX_LENGTH];

    if (path == NULL) {
        fprintf(stderr, "PATH environment variable not set.\n");
        exit(1);
    }

    // Get the home directory from the HOME environment variable
    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "HOME environment variable not set.\n");
        exit(1);
    }
    strncpy(home_dir, home, sizeof(home_dir) - 1);
    home_dir[sizeof(home_dir) - 1] = '\0'; // Ensure null termination

    while (1) {  // Infinite loop for REPL
        printf("$ ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            exit(0);  // Exit with status 0 on EOF
        }

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        if (!strcmp(input, "exit 0")) {
            exit(0);
        }

        // Tokenize the command with handling for quoted strings
        int argc = 0;
        handle_quotes(input, args, &argc);

        if (argc > 0) {
            if (is_builtin(args[0])) {
                // Handle built-ins
                if (strcmp(args[0], "type") == 0 && argc > 1) {
                    if (is_builtin(args[1])) {
                        printf("%s is a shell builtin\n", args[1]);
                    } else {
                        char *found_path = find_in_path(args[1], path);
                        if (found_path) {
                            printf("%s is %s\n", args[1], found_path);
                        } else {
                            printf("%s: not found\n", args[1]);
                        }
                    }
                } else if (strcmp(args[0], "echo") == 0) {
                    // Echo handling with single quotes support
                    for (int i = 1; i < argc; i++) {
                        printf("%s", args[i]);  // Print without extra space
                        if (i < argc - 1) printf(" "); // Add space only between arguments
                    }
                    printf("\n");
                } else if (strcmp(args[0], "pwd") == 0) {
                    // Print the current working directory
                    char cwd[PATH_MAX];
                    if (getcwd(cwd, sizeof(cwd)) != NULL) {
                        printf("%s\n", cwd);
                    } else {
                        perror("getcwd");
                        exit(1);
                    }
                } else if (strcmp(args[0], "cd") == 0 && argc == 2) {
                    // CD command for absolute paths and home directory
                    if (strcmp(args[1], "~") == 0) {
                        // Change to home directory
                        if (chdir(home_dir) != 0) {
                            printf("cd: ~: %s\n", strerror(errno));
                        }
                    } else if (chdir(args[1]) != 0) {
                        printf("cd: %s: %s\n", args[1], strerror(errno));
                    }
                }
            } else {
                    // Handle external commands
                    char *cmd_path = find_in_path(args[0], path);
                    if (cmd_path == NULL) {
                        // Command not found in PATH
                        printf("%s: command not found\n", args[0]);
                    } else {
                        pid_t pid = fork();
                        if (pid == 0) {  // Child process
                            execvp(args[0], args);
                            // If execvp returns, there was an error other than command not found
                            perror(args[0]);
                            exit(1);
                        } else if (pid > 0) {  // Parent process
                            int status;
                            waitpid(pid, &status, 0);  // Wait for child to complete
                        } else {
                            perror("fork");
                            exit(1);
                        }
                    }
            }
        } else {
            printf("Empty command\n");
        }

        // Free dynamically allocated memory for args
        for (int i = 0; i < argc; i++) {
            free(args[i]);
        }
    }

    return 0;
}

// Function implementations

// Helper function to handle quoted strings
void handle_quotes(char *input, char *args[], int *argc) {
    char token[PATH_MAX_LENGTH];
    int i = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    int token_start = 0;
    int concat_quotes = 0;

    for (int j = 0; input[j] != '\0'; j++) {
        if (input[j] == '\'' && !in_double_quotes) {
            in_single_quotes = !in_single_quotes;
            if (!in_single_quotes) {
                // End of single quote token
                token[i] = '\0';
                args[(*argc)++] = strdup(token);
                i = 0;
                concat_quotes = 0;  // Reset concatenation flag
            } else {
                token_start = j + 1;
            }
        } else if (input[j] == '"' && !in_single_quotes) {
            in_double_quotes = !in_double_quotes;
            if (!in_double_quotes) {
                // End of double quote token
                token[i] = '\0';
                if (!concat_quotes) {
                    args[(*argc)++] = strdup(token);
                }
                i = 0;
                concat_quotes = 0;  // Reset concatenation flag
            } else {
                token_start = j + 1;
            }
        } else if (input[j] == ' ' && !in_single_quotes && !in_double_quotes) {
            if (i > 0) {
                token[i] = '\0';
                args[(*argc)++] = strdup(token);
                i = 0;
            }
        } else {
            if (in_double_quotes && input[j] == '\\' && (input[j+1] == '\\' || input[j+1] == '"' || input[j+1] == '$' || input[j+1] == '\n')) {
                // Handle escape sequences in double quotes
                j++; // Skip the next character
                token[i++] = input[j]; // Add the escaped character
            } else {
                if (!in_single_quotes && !in_double_quotes && i == 0) {
                    token_start = j;
                }
                token[i++] = input[j];
            }

            // Handle concatenated quotes
            if ((in_single_quotes || in_double_quotes) && input[j + 1] == input[j] && !concat_quotes) {
                concat_quotes = 1; // Set flag to concatenate
            }
        }
    }

    // Handle the last token if any
    if (i > 0 || in_single_quotes || in_double_quotes) {
        token[i] = '\0';
        args[(*argc)++] = strdup(token);
    }
    args[*argc] = NULL;  // Null terminate the array
}

static int is_builtin(const char *command) {
    return (strcmp(command, "echo") == 0 ||
            strcmp(command, "exit") == 0 ||
            strcmp(command, "type") == 0 ||
            strcmp(command, "pwd") == 0 ||
            strcmp(command, "cd") == 0); // Add cd to builtins
}

char *find_in_path(const char *cmd, const char *path_env) {
    static char full_path[PATH_MAX_LENGTH];
    char *path_copy = strdup(path_env);
    char *dir = strtok(path_copy, ":");

    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

int tokenize_command(const char *cmd, char *args[]) {
    // This function is now replaced by handle_quotes above
    return 0; // Placeholder to compile
}

