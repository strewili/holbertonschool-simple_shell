#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * find_alias - Find an alias by name.
 * @shell: Shell state.
 * @name: Alias name.
 *
 * Return: Alias node, or NULL.
 */
alias_t *find_alias(shell_t *shell, char *name)
{
	alias_t *node;

	node = shell->aliases;
	while (node != NULL)
	{
		if (strcmp(node->name, name) == 0)
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * set_alias - Set or update an alias.
 * @shell: Shell state.
 * @name: Alias name.
 * @value: Alias value.
 *
 * Return: 0 on success, 1 on failure.
 */
int set_alias(shell_t *shell, char *name, char *value)
{
	alias_t *node;

	node = find_alias(shell, name);
	if (node == NULL)
	{
		node = malloc(sizeof(alias_t));
		if (node == NULL)
			return (1);
		node->name = strdup(name);
		node->value = NULL;
		node->next = shell->aliases;
		shell->aliases = node;
	}
	free(node->value);
	node->value = strdup(value);
	if (node->name == NULL || node->value == NULL)
		return (1);
	return (0);
}

/**
 * print_alias - Print one alias.
 * @alias: Alias node.
 */
void print_alias(alias_t *alias)
{
	if (alias != NULL)
		printf("%s='%s'\n", alias->name, alias->value);
}

/**
 * builtin_alias - Run alias builtin.
 * @shell: Shell state.
 * @args: Arguments.
 *
 * Return: 0 on success, 1 on failure.
 */
int builtin_alias(shell_t *shell, char **args)
{
	int index;
	char *equal;
	alias_t *node;

	if (args[1] == NULL)
	{
		for (node = shell->aliases; node != NULL; node = node->next)
			print_alias(node);
		return (0);
	}
	for (index = 1; args[index] != NULL; index++)
	{
		equal = strchr(args[index], '=');
		if (equal != NULL)
		{
			*equal = '\0';
			if (set_alias(shell, args[index], equal + 1) == 1)
				return (1);
		}
		else
			print_alias(find_alias(shell, args[index]));
	}
	return (0);
}

/**
 * join_alias - Join alias value and remaining arguments.
 * @alias: Alias value.
 * @args: Original arguments.
 *
 * Return: New allocated command line, or NULL.
 */
static char *join_alias(char *alias, char **args)
{
	char *line;
	int index;
	size_t len;

	len = strlen(alias) + 1;
	for (index = 1; args[index] != NULL; index++)
		len += strlen(args[index]) + 1;
	line = malloc(len);
	if (line == NULL)
		return (NULL);
	strcpy(line, alias);
	for (index = 1; args[index] != NULL; index++)
	{
		strcat(line, " ");
		strcat(line, args[index]);
	}
	return (line);
}

/**
 * apply_alias - Replace first argument when it is an alias.
 * @shell: Shell state.
 * @args: Arguments.
 *
 * Return: Argument vector.
 */
char **apply_alias(shell_t *shell, char **args)
{
	alias_t *alias;
	char *line;
	char **new_args;

	if (args == NULL || args[0] == NULL)
		return (args);
	alias = find_alias(shell, args[0]);
	if (alias == NULL)
		return (args);
	line = join_alias(alias->value, args);
	if (line == NULL)
		return (args);
	new_args = split_words(line);
	free(line);
	if (new_args == NULL)
		return (args);
	free_words(args);
	return (new_args);
}

/**
 * free_aliases - Free alias list.
 * @alias: Alias list.
 */
void free_aliases(alias_t *alias)
{
	alias_t *next;

	while (alias != NULL)
	{
		next = alias->next;
		free(alias->name);
		free(alias->value);
		free(alias);
		alias = next;
	}
}
