#include "shell.h"

/**
 * tokenize_input - Splits a string into tokens based on spaces
 * @line: The input string
 * Return: Array of pointers to tokens
 */
char **tokenize_input(char *line)
{
	char **tokens;
	char *token;
	int i = 0;

	tokens = malloc(sizeof(char *) * 64);
	if (!tokens)
		return (NULL);

	token = strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		tokens[i] = token;
		i++;
		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * check_builtin - Checks if the command is a built-in like exit
 * @args: Array of arguments
 * Return: 0 if exit is triggered, 1 otherwise
 */
int check_builtin(char **args)
{
	if (strcmp(args[0], "exit") == 0)
	{
		return (0);
	}
	return (1);
}

/**
 * execute_command - Forks a child process to execute an external command
 * @args: Array of arguments
 * Return: Always 1 to continue the shell loop
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
		{
			perror("./hsh");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		perror("./hsh");
	}
	else
	{
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return (1);
}
