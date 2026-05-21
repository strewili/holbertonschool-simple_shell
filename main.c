#include "shell.h"

/**
 * main - entry point for simple shell
 * @ac: argument count
 * @av: argument vector
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	int loop_count = 0;
	(void)ac;

	while (1)
	{
		loop_count++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		args = tokenize_input(line);
		if (args && args[0])
		{
			if (strcmp(args[0], "exit") == 0)
			{
				free_args(args);
				break;
			}
			execute_command(args, av[0], loop_count);
		}
		free_args(args);
	}
	free(line);
	return (0);
}

/**
 * _strlen - returns string length
 * @s: string
 * Return: length
 */
int _strlen(char *s)
{
	int i = 0;

	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

/**
 * _strdup - duplicates a string
 * @str: string to copy
 * Return: pointer to new string
 */
char *_strdup(char *str)
{
	char *duplicate;
	int i, len;

	if (str == NULL)
		return (NULL);
	len = _strlen(str);
	duplicate = malloc((len + 1) * sizeof(char));
	if (duplicate == NULL)
		return (NULL);
	for (i = 0; i < len; i++)
		duplicate[i] = str[i];
	duplicate[len] = '\0';
	return (duplicate);
}

/**
 * free_args - frees arguments array
 * @args: array to free
 */
void free_args(char **args)
{
	int i = 0;

	if (!args)
		return;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
