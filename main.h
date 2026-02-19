#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

void prompt(void);
char *trim_spaces(char *str);
char **split_line(char *line);
void run_command(char *line);

#endif /* MAIN_H */
