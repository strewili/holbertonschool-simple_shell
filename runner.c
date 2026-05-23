#include "shell.h"

#include <stdlib.h>

/**
 * token_end - Find the end of the next command segment.
 * @line: Input line.
 * @op: Operator found.
 *
 * Return: Index of operator or end of string.
 */
static int token_end(char *line, char *op)
{
	int index;

	*op = '\0';
	for (index = 0; line[index] != '\0'; index++)
	{
		if (line[index] == ';')
		{
			*op = ';';
			return (index);
		}
		if (line[index] == '&' && line[index + 1] == '&')
		{
			*op = '&';
			return (index);
		}
		if (line[index] == '|' && line[index + 1] == '|')
		{
			*op = '|';
			return (index);
		}
	}
	return (index);
}

/**
 * should_run - Decide whether a command should run.
 * @previous: Previous operator.
 * @status: Previous status.
 *
 * Return: 1 if command should run, otherwise 0.
 */
static int should_run(char previous, int status)
{
	if (previous == '&' && status != 0)
		return (0);
	if (previous == '|' && status == 0)
		return (0);
	return (1);
}

/**
 * run_command - Run one command segment.
 * @shell: Shell state.
 * @segment: Command segment.
 *
 * Return: Command status.
 */
static int run_command(shell_t *shell, char *segment)
{
	char *clean;
	char *expanded;
	char **args;
	int status;

	clean = strip_comments(segment);
	if (clean == NULL || clean[0] == '\0')
		return (shell->last_status);
	expanded = expand_vars(shell, clean);
	if (expanded == NULL)
		return (1);
	args = split_words(expanded);
	free(expanded);
	if (args == NULL)
		return (shell->last_status);
	args = apply_alias(shell, args);
	status = execute_args(shell, args);
	free_words(args);
	return (status);
}

/**
 * run_line - Run a full input line.
 * @shell: Shell state.
 * @line: Input line.
 *
 * Return: Last command status.
 */
int run_line(shell_t *shell, char *line)
{
	char previous;
	char op;
	int start;
	int end;
	int skip;

	previous = ';';
	start = 0;
	while (line[start] != '\0')
	{
		end = token_end(line + start, &op);
		skip = (op == '&' || op == '|') ? 2 : 1;
		line[start + end] = '\0';
		if (should_run(previous, shell->last_status))
			shell->last_status = run_command(shell, line + start);
		if (op == '\0')
			break;
		previous = op;
		start += end + skip;
	}
	return (shell->last_status);
}
