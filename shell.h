#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

#define FALSE 0
#define TRUE 1
#define NEITHER 2
#define MATCH 3
#define PREFIX 4
#define EXIT_SHELL 5
#define SKIP_FORK 6
#define DO_EXECVE 7

extern char **environ;

extern int status;

extern int line_num;

extern char *shell_name;

int command_manager(char **args);

int built_ins(char **args);

char *check_command(char **args);

int execute_command(char **args);

char *input_san(char *old_buf, size_t *old_size);

void err_message(char *arg0, char *arg1);

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

int _getline(char **line_ptr, size_t *n, int file);

int _strlen(char *str);

char *_strdup(char *src);

char *str_concat(char *s1, char *s2);

int str_compare(char *s1, char *s2, int pref_or_match);

char *get_array_element(char **array, char *element_name);

char **make_array(char *str, char delim, char **if_sep);

int list_len(char **list, char *entry);

char **array_cpy(char **old_array, int new_size);

int free_array(char **args);

int print_env(void);

#endif
