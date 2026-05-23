#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * history_path - Build the history file path.
 * @shell: Shell state.
 *
 * Return: Allocated path, or NULL.
 */
static char *history_path(shell_t *shell)
{
	char *home;
	char *path;

	home = _getenv("HOME");
	if (home == NULL)
		return (NULL);
	path = malloc(strlen(home) + strlen("/.simple_shell_history") + 1);
	if (path == NULL)
		return (NULL);
	sprintf(path, "%s/.simple_shell_history", home);
	shell->history_file = path;
	return (path);
}

/**
 * history_load - Load history from HOME.
 * @shell: Shell state.
 */
void history_load(shell_t *shell)
{
	FILE *file;
	char *line;
	size_t size;
	ssize_t read;
	int total;

	if (history_path(shell) == NULL)
		return;
	file = fopen(shell->history_file, "r");
	if (file == NULL)
		return;
	line = NULL;
	size = 0;
	total = 0;
	while ((read = getline(&line, &size, file)) != -1)
	{
		if (read > 0 && line[read - 1] == '\n')
			line[read - 1] = '\0';
		history_add(shell, line);
		total++;
	}
	shell->history_base = total % HIST_MAX;
	free(line);
	fclose(file);
}

/**
 * history_add - Add a command to history.
 * @shell: Shell state.
 * @line: Command line.
 */
void history_add(shell_t *shell, char *line)
{
	if (line == NULL || line[0] == '\0')
		return;
	if (shell->history_count == HIST_MAX)
	{
		free(shell->history[0]);
		memmove(shell->history, shell->history + 1,
			sizeof(char *) * (HIST_MAX - 1));
		shell->history_count--;
	}
	shell->history[shell->history_count] = strdup(line);
	if (shell->history[shell->history_count] != NULL)
		shell->history_count++;
}

/**
 * history_save - Save history to HOME.
 * @shell: Shell state.
 */
void history_save(shell_t *shell)
{
	FILE *file;
	int index;

	if (shell->history_file == NULL)
		return;
	file = fopen(shell->history_file, "w");
	if (file == NULL)
		return;
	for (index = 0; index < shell->history_count; index++)
		fprintf(file, "%s\n", shell->history[index]);
	fclose(file);
}

/**
 * print_history - Print command history.
 * @shell: Shell state.
 */
void print_history(shell_t *shell)
{
	int index;

	for (index = 0; index < shell->history_count; index++)
		printf("%d %s\n", shell->history_base + index,
			shell->history[index]);
}
