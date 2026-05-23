#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * init_shell - Initialize shell state.
 * @shell: Shell state.
 * @argv: Program arguments.
 * @input: Input stream.
 * @interactive: Interactive flag.
 */
void init_shell(shell_t *shell, char **argv, FILE *input, int interactive)
{
	int index;

	shell->name = argv[0];
	shell->input = input;
	shell->interactive = interactive;
	shell->line_number = 0;
	shell->last_status = 0;
	shell->aliases = NULL;
	shell->history_count = 0;
	shell->history_base = 0;
	shell->history_file = NULL;
	for (index = 0; index < HIST_MAX; index++)
		shell->history[index] = NULL;
	history_load(shell);
}

/**
 * shell_loop - Main shell read loop.
 * @shell: Shell state.
 *
 * Return: Last command status.
 */
int shell_loop(shell_t *shell)
{
	char *line;
	size_t size;
	ssize_t read;

	line = NULL;
	size = 0;
	while (1)
	{
		if (shell->interactive)
			write(STDOUT_FILENO, "$ ", 2);
		read = getline(&line, &size, shell->input);
		if (read == -1)
			break;
		shell->line_number++;
		if (read > 0 && line[read - 1] == '\n')
			line[read - 1] = '\0';
		history_add(shell, line);
		run_line(shell, line);
	}
	free(line);
	history_save(shell);
	if (shell->interactive)
		write(STDOUT_FILENO, "\n", 1);
	return (shell->last_status);
}

/**
 * free_shell - Free shell state memory.
 * @shell: Shell state.
 */
void free_shell(shell_t *shell)
{
	int index;

	free_aliases(shell->aliases);
	free(shell->history_file);
	for (index = 0; index < shell->history_count; index++)
		free(shell->history[index]);
}
