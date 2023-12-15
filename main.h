#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
void printenv(void);
int *_strconcat(char **buffer, char *s1, char *s2);
int handle_path_expetions(char **buffer_error, char *PATH, char* command, char* shellname);
char *handle_path(char *command, char *PATH, char* shellname, int *flag_free);
char *_getenv(char **env_var_line, char *name, int *);
int _strcmp(char *s1, char *s2);
int _strlen(char *s);
int _getline(char **buffer);
#endif
