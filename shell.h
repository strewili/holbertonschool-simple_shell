#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

char **tokenize_input(char *line);
char *find_path(char *command);
int execute_command(char **args, char *prog_name, int loop_count);
int _strlen(char *s);
char *_strdup(char *str);
void free_args(char **args);

#endif
