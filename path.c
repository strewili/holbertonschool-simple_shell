#include "shell.h"

/**
 * get_env_value - finds the value of an environment variable
 * @name: name of the variable
 * Return: pointer to the value, or NULL
 */
char *get_env_value(char *name)
{
	int i = 0;
	int len = _strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * find_path - looks for a command in the PATH directories
 * @command: command to find
 * Return: full path of the command, or NULL
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *token, *full_path;
	struct stat st;

	if (command == NULL)
		return (NULL);
	if (stat(command, &st) == 0)
		return (_strdup(command));
	path_env = get_env_value("PATH");
	if (!path_env || _strlen(path_env) == 0)
		return (NULL);
	path_copy = _strdup(path_env);
	if (!path_copy)
		return (NULL);
	token = strtok(path_copy, ":");
	while (token != NULL)
	{
		full_path = malloc(_strlen(token) + _strlen(command) + 2);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		strcpy(full_path, token);
		strcat(full_path, "/");
		strcat(full_path, command);
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
