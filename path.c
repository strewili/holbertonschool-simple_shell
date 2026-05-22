#include "shell.h"

/**
 * get_env_value - Gets the value of an environment variable
 * @name: Name of the variable
 * Return: Pointer to the value, or NULL if not found
 */
char *get_env_value(const char *name)
{
	int i = 0;
	size_t len = _strlen(name);

	while (environ[i])
	{
		if (_strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			return (environ[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}

/**
 * find_path - Finds the full path of a command using PATH env
 * @command: The command name
 * Return: Full path string, or NULL if not found
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *token, *full_path;
	struct stat st;
	int cmd_len, dir_len;

	if (!command)
		return (NULL);

	if (_strchr(command, '/') != NULL)
	{
		if (stat(command, &st) == 0)
			return (_strdup(command));
		return (NULL);
	}

	path_env = get_env_value("PATH");
	if (!path_env || _strlen(path_env) == 0)
		return (NULL);

	path_copy = _strdup(path_env);
	if (!path_copy)
		return (NULL);

	cmd_len = _strlen(command);
	token = _strtok(path_copy, ":");

	while (token)
	{
		dir_len = _strlen(token);
		full_path = malloc(dir_len + cmd_len + 2);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}

		{
			int i, j;

			for (i = 0; token[i]; i++)
				full_path[i] = token[i];
			full_path[i++] = '/';
			for (j = 0; command[j]; j++)
				full_path[i + j] = command[j];
			full_path[i + j] = '\0';
		}

		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = _strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
