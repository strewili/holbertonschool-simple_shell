#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

extern char **environ;

/**
 * struct shell_state - Runtime state for the simple shell.
 * @name: Shell program name.
 * @line: Current input line.
 * @line_number: Number of commands read.
 */
typedef struct shell_state
{
	char *name;
	char *line;
	unsigned int line_number;
} shell_state_t;

char **split_line(char *line);
void free_args(char **args);
int execute_command(char **args, shell_state_t *state);
void print_error(shell_state_t *state, char *command);

#endif
