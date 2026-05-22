#include "shell.h"

/**
 * my_getline - Reads an entire line from standard input
 * @lineptr: Buffer that stores the input line
 * @n: Size of the allocated buffer
 * @stream: Stream to read from (stdin)
 * Return: Number of characters read, or -1 on failure/EOF
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
		*n = 128;
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
		{
			*n *= 2;
			*lineptr = realloc(*lineptr, *n);
			if (*lineptr == NULL)
				return (-1);
		}
		(*lineptr)[count++] = c;
		if (c == '\n')
			break;
	}
	(*lineptr)[count] = '\0';
	return (count);
}
