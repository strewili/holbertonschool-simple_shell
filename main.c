#include "shell.h"

/**
 * main - Entry point for the simple shell
 * Return: Always 0 (Success)
 */
int main(void)
{
	run_shell();
	return (0);
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
	char *actual_command;
	int last_exit_status = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			exit(last_exit_status);
		}

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		args = tokenize_input(line);
		if (args && args[0])
		{
			if (strcmp(args[0], "exit") == 0)
			{
				free(args);
				free(line);
				exit(last_exit_status);
			}

			/* البحث عن المسار الحقيقي للأمر قبل الـ fork */
			actual_command = find_path(args[0]);
			if (actual_command != NULL)
			{
				free(args[0]);
				args[0] = actual_command; /* استبداله بالمسار الكامل */
				last_exit_status = execute_command(args);
			}
			else
			{
				/* طباعة الخطأ المطابق لـ sh بدون استدعاء fork */
				fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
				last_exit_status = 127;
			}
		}
		free(args);
	}
	free(line);
}
