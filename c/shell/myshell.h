// myshell.h
#ifndef MYSHELL_H
#define MYSHELL_H

#define MAX_INPUT 256
#define MAX_ARGS 32

int parse_input(char *input, char **args, char **input_file, char **output_file, int *append);
int execute_builtin(char **args, int argc);
void execute_command(char **args, char *input_file, char *output_file, int append);

#endif
