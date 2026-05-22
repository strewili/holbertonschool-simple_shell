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

void run_shell(void);
char **tokenize_input(char *line);
int execute_command(char **args);
int check_builtin(char **args);
char *get_env_value(const char *name);
char *find_path(char *command);

#endif /* SHELL_H */
