#include "shell.h"

/**
 * tokenize_input - splits input line into tokens
 * @line: string input
 * Return: array of tokens
 */
char **tokenize_input(char *line)
{
	char **args;
	char *token;
	int i = 0, bufsize = 64;

	if (!line)
		return (NULL);
	args = malloc(bufsize * sizeof(char *));
	if (!args)
		return (NULL);
	token = strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		args[i] = _strdup(token);
		i++;
		if (i >= bufsize)
		{
			bufsize += 64;
			args = realloc(args, bufsize * sizeof(char *));
			if (!args)
				return (NULL);
		}
		token = strtok(NULL, " \t\r\n\a");
	}
	args[i] = NULL;
	return (args);
}

/**
 * check_builtin - checks if command is a builtin
 * @args: arguments array
 * Return: 1 if exit built-in handles execution termination, 0 otherwise
 */
int check_builtin(char **args)
{
	if (!args || !args[0])
		return (0);
	if (strcmp(args[0], "exit") == 0)
		return (1);
	return (0);
}

/**
 * execute_command - forks and executes given command
 * @args: arguments array
 * @prog_name: shell program name
 * @loop_count: line execution count
 * Return: 0 on success, 127 if not found
 */
int execute_command(char **args, char *prog_name, int loop_count)
{
	char *actual_path;
	pid_t child_pid;
	int status;

	if (!args || !args[0])
		return (0);
	actual_path = find_path(args[0]);
	if (!actual_path)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
				prog_name, loop_count, args[0]);
		return (127);
	}
	child_pid = fork();
	if (child_pid == 0)
	{
		if (execve(actual_path, args, environ) == -1)
		{
			perror("Error");
			free(actual_path);
			free_args(args);
			exit(1);
		}
	}
	else if (child_pid < 0)
	{
		perror("Fork failed");
	}
	else
	{
		wait(&status);
	}
	free(actual_path);
	return (0);
}
