#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char **fill_args(char *line);
int _strcmp(char *s1, char *s2);
void hsh_exit(char **args);
int execute_command(char **args);

#endif /* MAIN_H */
