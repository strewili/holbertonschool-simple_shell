#include "shell.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * add_text - Append text to a dynamic string.
 * @result: Current result.
 * @len: Current length.
 * @text: Text to append.
 *
 * Return: New result, or NULL.
 */
static char *add_text(char *result, size_t *len, char *text)
{
	size_t add;
	char *new_result;

	add = strlen(text);
	new_result = realloc(result, *len + add + 1);
	if (new_result == NULL)
	{
		free(result);
		return (NULL);
	}
	memcpy(new_result + *len, text, add);
	*len += add;
	new_result[*len] = '\0';
	return (new_result);
}

/**
 * var_value - Get replacement text for a variable.
 * @shell: Shell state.
 * @name: Variable name.
 *
 * Return: Replacement value.
 */
static char *var_value(shell_t *shell, char *name)
{
	static char buffer[32];
	char *value;

	if (strcmp(name, "?") == 0)
	{
		sprintf(buffer, "%d", shell->last_status);
		return (buffer);
	}
	if (strcmp(name, "$") == 0)
	{
		sprintf(buffer, "%d", getpid());
		return (buffer);
	}
	value = _getenv(name);
	if (value == NULL)
		return ("");
	return (value);
}

/**
 * expand_vars - Replace shell variables in a line.
 * @shell: Shell state.
 * @line: Input line.
 *
 * Return: New expanded string.
 */
char *expand_vars(shell_t *shell, char *line)
{
	char *result;
	char name[128];
	size_t len;
	int index;
	int nindex;

	result = malloc(1);
	if (result == NULL)
		return (NULL);
	result[0] = '\0';
	len = 0;
	for (index = 0; line[index] != '\0'; index++)
	{
		if (line[index] != '$')
		{
			name[0] = line[index];
			name[1] = '\0';
			result = add_text(result, &len, name);
			continue;
		}
		index++;
		nindex = 0;
		if (line[index] == '?' || line[index] == '$')
			name[nindex++] = line[index];
		else
			while (isalnum((unsigned char)line[index]) ||
			       line[index] == '_')
				name[nindex++] = line[index++];
		name[nindex] = '\0';
		if (nindex == 0)
		{
			result = add_text(result, &len, "$");
			index--;
			continue;
		}
		if (line[index] == '\0')
			index--;
		result = add_text(result, &len, var_value(shell, name));
	}
	return (result);
}
