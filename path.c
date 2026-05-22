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
 * _setenv - Initializes a new environment variable, or modifies an existing one
 * @variable: Name of the variable
 * @value: Value to set
 * Return: 0 on success, -1 on failure
 */
int _setenv(const char *variable, const char *value)
{
	int i = 0;
	size_t var_len = _strlen(variable);
	size_t val_len = _strlen(value);
	char *new_env;
	/* مصفوفة مخصصة لتتبع النصوص المحجوزة يدوياً وتحريرها بأمان */
	static char *my_allocs[256];
	static int alloc_count = 0;
	int k;

	if (!variable || !value)
		return (-1);

	new_env = malloc(var_len + val_len + 2);
	if (!new_env)
		return (-1);

	{
		size_t j, m;

		for (j = 0; variable[j]; j++)
			new_env[j] = variable[j];
		new_env[j++] = '=';
		for (m = 0; value[m]; m++)
			new_env[j + m] = value[m];
		new_env[j + m] = '\0';
	}

	while (environ[i])
	{
		if (_strncmp(environ[i], variable, var_len) == 0 && environ[i][var_len] == '=')
		{
			/* فحص ما إذا كنا نحن من قمنا بحجز هذا المؤشر سابقاً لنقوم بتحريره */
			for (k = 0; k < alloc_count; k++)
			{
				if (my_allocs[k] == environ[i])
				{
					free(environ[i]);
					my_allocs[k] = new_env;
					break;
				}
			}
			if (k == alloc_count && alloc_count < 256)
			{
				my_allocs[alloc_count++] = new_env;
			}
			environ[i] = new_env;
			return (0);
		}
		i++;
	}

	environ[i] = new_env;
	environ[i + 1] = NULL;
	if (alloc_count < 256)
		my_allocs[alloc_count++] = new_env;

	return (0);
}

/**
 * _unsetenv - Removes an environment variable from the system
 * @variable: Name of the variable to remove
 * Return: 0 on success, -1 on failure
 */
int _unsetenv(const char *variable)
{
	int i = 0, j;
	size_t var_len = _strlen(variable);

	if (!variable)
		return (-1);

	while (environ[i])
	{
		if (_strncmp(environ[i], variable, var_len) == 0 && environ[i][var_len] == '=')
		{
			j = i;
			while (environ[j])
			{
				environ[j] = environ[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (0);
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
