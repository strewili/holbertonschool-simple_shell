#include "shell.h"

#include <stdlib.h>
#include <string.h>

/**
 * next_word - Extract the next shell word.
 * @cursor: Current scan position.
 *
 * Return: Next word, or NULL.
 */
static char *next_word(char **cursor)
{
	char *start;
	char *read;
	char *write;
	char quote;

	while (**cursor == ' ' || **cursor == '\t' || **cursor == '\r')
		(*cursor)++;
	if (**cursor == '\0' || **cursor == '\n')
		return (NULL);
	start = *cursor;
	read = start;
	write = start;
	quote = '\0';
	while (*read != '\0')
	{
		if (quote == '\0' && (*read == ' ' || *read == '\t' ||
		    *read == '\r' || *read == '\n'))
			break;
		if ((*read == '\'' || *read == '"') &&
		    (quote == '\0' || quote == *read))
		{
			quote = (quote == '\0') ? *read : '\0';
			read++;
			continue;
		}
		*write++ = *read++;
	}
	if (*read != '\0')
		read++;
	*write = '\0';
	*cursor = read;
	return (start);
}

/**
 * strip_comments - Remove comments from a line.
 * @line: Input line.
 *
 * Return: The same line without comment text.
 */
char *strip_comments(char *line)
{
	int index;

	for (index = 0; line[index] != '\0'; index++)
	{
		if (line[index] == '#' && (index == 0 || line[index - 1] == ' '))
		{
			line[index] = '\0';
			break;
		}
	}
	return (line);
}

/**
 * split_words - Split a line into arguments.
 * @line: Input line.
 *
 * Return: Argument vector, or NULL.
 */
char **split_words(char *line)
{
	char **args;
	char **new_args;
	char *token;
	char *cursor;
	int count;
	int capacity;

	count = 0;
	capacity = 8;
	args = malloc(sizeof(char *) * capacity);
	if (args == NULL)
		return (NULL);
	cursor = line;
	token = next_word(&cursor);
	while (token != NULL)
	{
		if (count + 1 >= capacity)
		{
			capacity *= 2;
			new_args = realloc(args, sizeof(char *) * capacity);
			if (new_args == NULL)
			{
				args[count] = NULL;
				free_words(args);
				return (NULL);
			}
			args = new_args;
		}
		args[count] = strdup(token);
		if (args[count] == NULL)
		{
			args[count] = NULL;
			free_words(args);
			return (NULL);
		}
		count++;
		token = next_word(&cursor);
	}
	args[count] = NULL;
	if (count == 0)
	{
		free(args);
		return (NULL);
	}
	return (args);
}

/**
 * free_words - Free argument vector.
 * @words: Argument vector.
 */
void free_words(char **words)
{
	int index;

	if (words == NULL)
		return;
	for (index = 0; words[index] != NULL; index++)
		free(words[index]);
	free(words);
}
