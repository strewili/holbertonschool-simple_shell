#include "shell.h"

/**
 * main - Entry point for the simple shell
 * Return: Always 0 (Success)
 */
int main(void)
{
	signal(SIGINT, handle_sigint);
	init_environ(); /* تجهيز نسخة بيئة ديناميكية قابلة للتعديل والمسح */
	run_shell();
	return (0);
}

/**
 * init_environ - Creates a dynamic copy of the environ array on the heap
 */
void init_environ(void)
{
	char **new_env;
	int i = 0, count = 0;

	while (environ[count])
		count++;

	/* حجز مساحة لـ 1024 متغير ليعطينا أمان كامل عند إضافة متغيرات جديدة مسبقاً */
	new_env = malloc(sizeof(char *) * 1024);
	if (!new_env)
		return;

	for (i = 0; i < count; i++)
	{
		new_env[i] = _strdup(environ[i]);
	}
	new_env[i] = NULL;
	environ = new_env;
}

/**
 * handle_sigint - Handles Ctrl+C (SIGINT) signal
 * @sig: The signal number
 */
void handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n($) ", 5);
}

/**
 * free_args - Frees double pointer array of strings safely
 * @args: Array of arguments
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

/**
 * run_shell - Runs the infinite loop of the shell interpreter
 */
void run_shell(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	char *trimmed_line;
	char *actual_command;
	int last_exit_status = 0;
	int builtin_status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		nread = my_getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			exit(last_exit_status);
		}

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		trimmed_line = trim_spaces(line);
		if (_strlen(trimmed_line) == 0)
			continue;

		args = tokenize_input(trimmed_line);
		if (args && args[0])
		{
			builtin_status = check_builtin(args, last_exit_status, line);
			if (builtin_status == 0)
			{
				free_args(args);
				continue;
			}
			else if (builtin_status == 2)
			{
				last_exit_status = 2;
				free_args(args);
				continue;
			}

			actual_command = find_path(args[0]);
			if (actual_command != NULL)
			{
				free(args[0]);
				args[0] = actual_command;
				last_exit_status = execute_command(args);
			}
			else
			{
				fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
				last_exit_status = 127;
			}
		}
		free_args(args);
	}
	free(line);
}
