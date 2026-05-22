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
	int status = 1;
	int last_exit_status = 0; /* متغير لحفظ حالة آخر أمر */

	while (status)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			exit(last_exit_status); /* الخروج بآخر حالة عند نهاية الملف */
		}

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		args = tokenize_input(line);
		if (args && args[0])
		{
			/* نمرر الـ last_exit_status للـ builtin */
			if (strcmp(args[0], "exit") == 0)
			{
				free(args);
				free(line);
				exit(last_exit_status);
			}
			
			/* تنفيذ الأمر العادي وتحديث الـ status والحالة الأخيرة */
			last_exit_status = execute_command(args);
		}
		free(args);
	}
	free(line);
}
