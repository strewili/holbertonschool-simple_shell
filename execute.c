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
 * check_builtin - Checks and executes built-in commands
 * @args: Array of arguments
 * @last_status: The exit status of the last command
 * @line: Input line pointer to free on exit
 * Return: 0 if executed, 1 otherwise, 2 if illegal status
 */
int check_builtin(char **args, int last_status, char *line)
{
	int custom_status;

	if (_strcmp(args[0], "exit") == 0)
	{
		custom_status = last_status;
		if (args[1] != NULL)
		{
			custom_status = _atoi(args[1]);
			if (custom_status == -1)
			{
				fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n",
					args[1]);
				if (!isatty(STDIN_FILENO))
				{
					free_args(args);
					free(line);
					exit(2);
				}
				return (2);
			}
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
	if (_strcmp(args[0], "setenv") == 0)
	{
		if (args[1] && args[2])
		{
			if (_setenv(args[1], args[2]) == -1)
				fprintf(stderr, "./hsh: setenv failed\n");
		}
		else
			fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (0);
	}
	if (_strcmp(args[0], "unsetenv") == 0)
	{
		if (args[1])
		{
			if (_unsetenv(args[1]) == -1)
				fprintf(stderr, "./hsh: unsetenv failed\n");
		}
		else
			fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (0);
	}
	if (_strcmp(args[0], "cd") == 0)
	{
		if (_cd(args) == -1)
			fprintf(stderr, "./hsh: cd failed\n");
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

/**
 * get_env_value - Gets the value of an environment variable
 * @name: Name of the environment variable
 *
 * Return: Pointer to the value, or NULL
 */
char *get_env_value(const char *name)
{
	int i;
	int len;

	if (name == NULL)
		return (NULL);

	len = _strlen(name);
	for (i = 0; environ[i] != NULL; i++)
	{
		if (_strncmp(environ[i], name, len) == 0 &&
		    environ[i][len] == '=')
			return (environ[i] + len + 1);
	}
	return (NULL);
}

/**
 * find_path - Finds command full path using PATH
 * @command: Command name
 *
 * Return: Full path allocated with malloc, or NULL
 */
char *find_path(char *command)
{
	char *path;
	char *path_copy;
	char *dir;
	char *full_path;
	int len;

	if (command == NULL)
		return (NULL);

	if (_strchr(command, '/') != NULL)
		return (_strdup(command));

	path = get_env_value("PATH");
	if (path == NULL)
		return (NULL);

	path_copy = _strdup(path);
	if (path_copy == NULL)
		return (NULL);

	dir = _strtok(path_copy, ":");
	while (dir != NULL)
	{
		len = _strlen(dir) + _strlen(command) + 2;
		full_path = malloc(sizeof(char) * len);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}

		full_path[0] = '\0';
		strcat(full_path, dir);
		strcat(full_path, "/");
		strcat(full_path, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = _strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
