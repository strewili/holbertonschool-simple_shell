#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * builtin_env - Print environment.
 *
 * Return: 0.
 */
static int builtin_env(void)
{
	int index;

	for (index = 0; environ[index] != NULL; index++)
		printf("%s\n", environ[index]);
	return (0);
}

/**
 * builtin_setenv - Set environment variable.
 * @args: Arguments.
 *
 * Return: 0 on success, 1 on failure.
 */
static int builtin_setenv(char **args)
{
	if (args[1] == NULL || args[2] == NULL)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (1);
	}
	if (setenv(args[1], args[2], 1) == -1)
	{
		perror("setenv");
		return (1);
	}
	return (0);
}

/**
 * builtin_unsetenv - Unset environment variable.
 * @args: Arguments.
 *
 * Return: 0 on success, 1 on failure.
 */
static int builtin_unsetenv(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (1);
	}
	if (unsetenv(args[1]) == -1)
	{
		perror("unsetenv");
		return (1);
	}
	return (0);
}

/**
 * builtin_cd - Change current directory.
 * @args: Arguments.
 *
 * Return: 0 on success, 1 on failure.
 */
static int builtin_cd(char **args)
{
	char cwd[1024];
	char *target;
	char *oldpwd;

	oldpwd = getcwd(cwd, sizeof(cwd));
	target = args[1];
	if (target == NULL)
		target = _getenv("HOME");
	else if (strcmp(target, "-") == 0)
		target = _getenv("OLDPWD");
	if (target == NULL)
		target = ".";
	if (chdir(target) == -1)
	{
		perror("cd");
		return (1);
	}
	if (oldpwd != NULL)
		setenv("OLDPWD", oldpwd, 1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		setenv("PWD", cwd, 1);
	return (0);
}

/**
 * builtin_help - Print builtin help.
 * @args: Arguments.
 *
 * Return: 0.
 */
static int builtin_help(char **args)
{
	(void)args;
	printf("Builtins: exit env setenv unsetenv cd alias help history\n");
	return (0);
}

/**
 * run_builtin - Run a builtin command when matched.
 * @shell: Shell state.
 * @args: Arguments.
 * @done: Set to 1 when command is builtin.
 *
 * Return: Builtin status.
 */
int run_builtin(shell_t *shell, char **args, int *done)
{
	*done = 1;
	if (strcmp(args[0], "exit") == 0)
	{
		history_save(shell);
		free_shell(shell);
		exit(shell->last_status);
	}
	if (strcmp(args[0], "env") == 0)
		return (builtin_env());
	if (strcmp(args[0], "setenv") == 0)
		return (builtin_setenv(args));
	if (strcmp(args[0], "unsetenv") == 0)
		return (builtin_unsetenv(args));
	if (strcmp(args[0], "cd") == 0)
		return (builtin_cd(args));
	if (strcmp(args[0], "alias") == 0)
		return (builtin_alias(shell, args));
	if (strcmp(args[0], "help") == 0)
		return (builtin_help(args));
	if (strcmp(args[0], "history") == 0)
	{
		print_history(shell);
		return (0);
	}
	*done = 0;
	return (0);
}
