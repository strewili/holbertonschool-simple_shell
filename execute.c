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
		/* حجز ذاكرة لكل وسيط وتجنب مشاكل الإشارة للذاكرة الأصلية */
		tokens[i] = strdup(token);
		i++;
		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * execute_command - Forks a child process to execute an external command
 * @args: Array of arguments
 * Return: The exit status of the executed command
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;
	int exit_code = 0;

	pid = fork();
	if (pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
		{
			perror("./hsh");
			exit(2);
		}
	}
	else if (pid < 0)
	{
		perror("./hsh");
		return (1);
	}
	else
	{
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
	}

	return (exit_code);
}
