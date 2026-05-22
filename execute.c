#include "shell.h"

/**
 * tokenize_input - Splits a string into tokens using our custom _strtok
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

	token = _strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		tokens[i] = _strdup(token);
		if (!tokens[i])
		{
			while (i > 0)
				free(tokens[--i]);
			free(tokens);
			return (NULL);
		}
		i++;
		token = _strtok(NULL, " \t\r\n\a");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * print_env - Prints the current environment variables
 */
void print_env(void)
{
	int i = 0;

	while (environ[i])
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/**
 * check_builtin - Checks and executes built-in commands like exit and env
 * @args: Array of arguments
 * @last_status: The exit status of the last command
 * @line: Input line pointer to free on exit
 * Return: 0 if a built-in was executed, 1 otherwise
 */
int check_builtin(char **args, int last_status, char *line)
{
	int custom_status;

	if (_strcmp(args[0], "exit") == 0)
	{
		custom_status = last_status;
		/* إذا كان هناك رقم ممرر مع الـ exit نقوم بمعالجته */
		if (args[1] != NULL)
		{
			custom_status = _atoi(args[1]);
		}
		free_args(args);
		free(line);
		exit(custom_status);
	}
	if (_strcmp(args[0], "env") == 0)
	{
		print_env();
		return (0);
	}
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
