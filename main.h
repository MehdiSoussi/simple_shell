#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
char *handle_path(char *command, char *PATH, char* shellname);
char *_strcat(char *dest, char *src);
char *_getenv(char *name);
int _strcmp(char *s1, char *s2);
int _strlen(char *);
char *path_concatenate(char *token, char* command);
void printenv(char **envp);
char *duplicate_string(char *environmentvariable);
char *_strconcat(char *s1, char *s2);
void handle_path_expetions(char *PATH, char* command, char* shellname);
#endif
