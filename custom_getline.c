#include "shell.h"

/**
 * my_getline - Reads an entire line from standard input
 * @lineptr: Buffer that stores the input line
 * @n: Size of the allocated buffer
 * @stream: Stream to read from
 * Return: Number of characters read
 */
ssize_t my_getline(char **lineptr, size_t *n, FILE *stream)
{
	static char buffer[1024];
	static size_t buffer_pos;
	static size_t buffer_size;
	size_t count = 0;
	char c;

	if (lineptr == NULL || n == NULL || stream == NULL)
		return (-1);

	if (*lineptr == NULL || *n == 0)
	{
		*n = 4096;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}

	while (1)
	{
		if (buffer_pos >= buffer_size)
		{
			buffer_size = read(STDIN_FILENO, buffer, 1024);
			buffer_pos = 0;
			if (buffer_size <= 0)
			{
				if (count == 0)
					return (-1);
				break;
			}
		}

		c = buffer[buffer_pos++];
		if (count >= *n - 1)
			break;

		(*lineptr)[count++] = c;
		if (c == '\n')
			break;
	}
	(*lineptr)[count] = '\0';
	return (count);
}

int _strlen(const char *s)
{
	int len = 0;

	while (s[len])
		len++;
	return (len);
}

int _strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i = 0;

	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && i < n - 1)
	{
		if (s1[i] != s2[i])
			break;
		i++;
	}
	return (s1[i] - s2[i]);
}

char *_strdup(const char *s)
{
	char *dup;
	int i, len = 0;

	if (!s)
		return (NULL);
	while (s[len])
		len++;
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	for (i = 0; i <= len; i++)
		dup[i] = s[i];
	return (dup);
}

char *_strchr(const char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

char *_strtok(char *str, const char *delim)
{
	static char *next_token;
	char *token_start;

	if (str != NULL)
		next_token = str;
	if (next_token == NULL || *next_token == '\0')
		return (NULL);

	while (*next_token && _strchr(delim, *next_token))
		next_token++;

	if (*next_token == '\0')
		return (NULL);

	token_start = next_token;
	while (*next_token && !_strchr(delim, *next_token))
		next_token++;

	if (*next_token)
	{
		*next_token = '\0';
		next_token++;
	}
	return (token_start);
}

char *trim_spaces(char *str)
{
	char *end;

	while (*str == ' ' || *str == '\t')
		str++;

	if (*str == '\0')
		return (str);

	end = str + _strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
	{
		*end = '\0';
		end--;
	}

	return (str);
}

/**
 * _atoi - Custom implementation to convert a string to an integer
 * @s: The string to convert
 * Return: The integer value parsed
 */
int _atoi(char *s)
{
	int i = 0;
	unsigned int res = 0;
	int sign = 1;

	while (s[i])
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			res = (res * 10) + (s[i] - '0');
		}
		else if (res > 0)
		{
			break;
		}
		i++;
	}
	return (res * sign);
}
