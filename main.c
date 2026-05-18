#define _POSIX_C_SOURCE 200809L

#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * main - Entry point for the simple shell.
 * @argc: Argument count.
 * @argv: Argument vector.
 *
 * Return: Always 0.
 */
int main(int argc, char **argv)
{
	shell_state_t state;
	size_t size;
	(void)argc;

	state.name = argv[0];
	state.line = NULL;
	state.line_number = 0;
	size = 0;

	while (1)
	{
		ssize_t read;
		char **args;

		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);
		read = getline(&state.line, &size, stdin);
		if (read == -1)
			break;
		state.line_number++;
		args = split_line(state.line);
		if (args == NULL)
			continue;
		execute_command(args, &state);
		free_args(args);
	}
	free(state.line);
	return (0);
}

/**
 * execute_command - Execute a command with arguments.
 * @args: Command and argument vector.
 * @state: Current shell state.
 *
 * Return: 0 on success, -1 on failure.
 */
int execute_command(char **args, shell_state_t *state)
{
	pid_t child;
	int status;

	if (args[0] == NULL)
		return (0);
	child = fork();
	if (child == -1)
	{
		perror(state->name);
		return (-1);
	}
	if (child == 0)
	{
		if (execve(args[0], args, environ) == -1)
		{
			print_error(state, args[0]);
			exit(127);
		}
	}
	else
	{
		wait(&status);
	}
	return (0);
}

/**
 * print_error - Print command-not-found error.
 * @state: Current shell state.
 * @command: Command that failed.
 */
void print_error(shell_state_t *state, char *command)
{
	fprintf(stderr, "%s: %u: %s: not found\n",
		state->name, state->line_number, command);
}

/**
 * free_args - Free an argument vector.
 * @args: Argument vector to free.
 */
void free_args(char **args)
{
	free(args);
}
