#ifndef SHELL_H
#define SHELL_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/types.h>

#define HIST_MAX 4096

extern char **environ;

typedef struct alias_s
{
	char *name;
	char *value;
	struct alias_s *next;
} alias_t;

typedef struct shell_s
{
	char *name;
	FILE *input;
	int interactive;
	unsigned int line_number;
	int last_status;
	alias_t *aliases;
	char *history[HIST_MAX];
	int history_count;
	int history_base;
	char *history_file;
} shell_t;

void init_shell(shell_t *shell, char **argv, FILE *input, int interactive);
void free_shell(shell_t *shell);
int shell_loop(shell_t *shell);
int run_line(shell_t *shell, char *line);
char **split_words(char *line);
void free_words(char **words);
char *strip_comments(char *line);
char *expand_vars(shell_t *shell, char *line);
char **apply_alias(shell_t *shell, char **args);
int execute_args(shell_t *shell, char **args);
int run_builtin(shell_t *shell, char **args, int *done);
int builtin_alias(shell_t *shell, char **args);
char *find_path(char *command);
char *_getenv(const char *name);
int set_alias(shell_t *shell, char *name, char *value);
alias_t *find_alias(shell_t *shell, char *name);
void print_alias(alias_t *alias);
void free_aliases(alias_t *alias);
void history_load(shell_t *shell);
void history_add(shell_t *shell, char *line);
void history_save(shell_t *shell);
void print_history(shell_t *shell);
void shell_error(shell_t *shell, char *msg);
void command_error(shell_t *shell, char *command, char *msg);

#endif
