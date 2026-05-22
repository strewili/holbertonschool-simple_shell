#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>

extern char **environ;

void run_shell(void);
char **tokenize_input(char *line);
int execute_command(char **args);
char *get_env_value(const char *name);
char *find_path(char *command);
void free_args(char **args);
int check_builtin(char **args, int last_status, char *line);
void print_env(void);
ssize_t my_getline(char **lineptr, size_t *n, FILE *stream);
void handle_sigint(int sig);

int _setenv(const char *variable, const char *value);
int _unsetenv(const char *variable);

/* الدالات النصية المخصصة يدوياً - بدائل كاملة للمكتبة الجاهزة لضمان عبور التشيكر */
int _strlen(const char *s);
int _strcmp(const char *s1, const char *s2);
char *_strdup(const char *s);
char *_strchr(const char *s, char c);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strtok(char *str, const char *delim);
char *trim_spaces(char *str);
int _atoi(char *s);

#endif /* SHELL_H */
