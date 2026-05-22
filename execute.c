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
 * @last_status: The exit status of the last executed command
 * Return: 0 if exit is triggered, 1 otherwise
 */
int check_builtin(char **args, int last_status)
{
	if (strcmp(args[0], "exit") == 0)
	{
		return (0);
	}
	(void)last_status;
	return (1);
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
			exit(2); /* خروج برقم 2 في حال لم يجد الأمر */
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

		/* لقط رقم الخروج الحقيقي للعملية الابنة */
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
	}

	return (exit_code);
}
