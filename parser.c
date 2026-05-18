#include "shell.h"

#include <stdlib.h>
#include <string.h>

/**
 * split_line - Split a command line into arguments.
 * @line: Input line.
 *
 * Return: NULL on failure or blank input, otherwise an argument vector.
 */
char **split_line(char *line)
{
	char **args;
	char *token;
	size_t count;
	size_t capacity;

	count = 0;
	capacity = 8;
	args = malloc(sizeof(char *) * capacity);
	if (args == NULL)
		return (NULL);

	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		if (count + 1 >= capacity)
		{
			char **new_args;

			capacity *= 2;
			new_args = realloc(args, sizeof(char *) * capacity);
			if (new_args == NULL)
			{
				free(args);
				return (NULL);
			}
			args = new_args;
		}
		args[count] = token;
		count++;
		token = strtok(NULL, " \t\r\n");
	}
	args[count] = NULL;

	if (count == 0)
	{
		free(args);
		return (NULL);
	}
	return (args);
}
