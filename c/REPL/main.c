#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to check if a command is a builtin
static int is_builtin(const char *command) {
    return (strcmp(command, "echo") == 0 ||
            strcmp(command, "exit") == 0 ||
            strcmp(command, "type") == 0);
}

int main() {
    setbuf(stdout, NULL);  // Initial flush to ensure output is unbuffered

    char input[100];
    char command[100];

    while (1) {  // Infinite loop for REPL
        // Print the prompt
        printf("$ ");

        // Flush after each prompt to ensure immediate output
        fflush(stdout);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // If reading fails (e.g., EOF), exit the loop
            printf("\n");
            exit(0);  // Exit with status 0 on EOF
        }

        // Remove the trailing newline if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // Check for commands
        if (strncmp(input, "type ", 5) == 0) {
            // Type command found, process the command after "type "
            sscanf(input + 5, "%s", command); // Extract the command to check

            if (is_builtin(command)) {
                printf("%s is a shell builtin\n", command);
            } else {
                printf("%s: not found\n", command);
            }
        } else if (strncmp(input, "echo ", 5) == 0) {
            // Echo command
            printf("%s\n", input + 5);  // +5 to skip "echo "
        } else if (strcmp(input, "exit 0") == 0) {
            exit(0);  // Exit with status 0
        } else {
            // Process other commands (since all other commands are invalid in this scenario)
            printf("%s: command not found\n", input);
        }
    }

    return 0; // This return statement is technically unreachable due to the while(1) loop
}
