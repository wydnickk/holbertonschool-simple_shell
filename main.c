#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ; /* For execve environment */

/**
 * prompt - display shell prompt only if interactive
 */
void prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		printf("#cisfun$ ");
		fflush(stdout);
	}
}

/**
 * trim_spaces - remove leading and trailing spaces/tabs
 * @str: input string
 *
 * Return: pointer to trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	if (str == NULL)
		return (NULL);

	while (*str == ' ' || *str == '\t')
		str++;

	if (*str == '\0')
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;

	*(end + 1) = '\0';
	return (str);
}

/**
 * split_line - split line into arguments (space-separated)
 * @line: input line
 *
 * Return: NULL-terminated array of strings
 */
char **split_line(char *line)
{
	int i, count = 0;
	char **args;
	char *p = line;

	while (*p)
	{
		while (*p == ' ' || *p == '\t')
			p++;
		if (*p)
		{
			count++;
			while (*p && *p != ' ' && *p != '\t')
				p++;
		}
	}

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);

	i = 0;
	p = line;
	while (*p)
	{
		while (*p == ' ' || *p == '\t')
			p++;
		if (*p)
		{
			args[i++] = p;
			while (*p && *p != ' ' && *p != '\t')
				p++;
			if (*p)
				*p++ = '\0';
		}
	}
	args[i] = NULL;
	return (args);
}

/**
 * run_command - execute a command with arguments
 * @line: command line
 */
void run_command(char *line)
{
	pid_t pid;
	char **args;
	char *cmd;
	char *path = NULL;
	char *env_path, *dir, *path_copy;
	char full[1024];

	cmd = trim_spaces(line);
	if (cmd == NULL || *cmd == '\0')
		return;

	args = split_line(cmd);
	if (!args || !args[0])
	{
		free(args);
		return;
	}

	/* PATH handle */
	if (strchr(args[0], '/'))
	{
		if (access(args[0], X_OK) == 0)
			path = strdup(args[0]);
	}
	else
	{
		env_path = getenv("PATH");
		if (env_path)
		{
			path_copy = strdup(env_path);
			dir = strtok(path_copy, ":");
			while (dir)
			{
				snprintf(full, sizeof(full), "%s/%s", dir, args[0]);
				if (access(full, X_OK) == 0)
				{
					path = strdup(full);
					break;
				}
				dir = strtok(NULL, ":");
			}
			free(path_copy);
		}
	}

	if (!path)
	{
		printf("%s: not found\n", args[0]);
		free(args);
		return;
	}

	/* fork only if command exists */
	pid = fork();
	if (pid == -1)
	{
		perror("Error");
		free(args);
		free(path);
		return;
	}

	if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
			perror("./hsh");
		exit(1);
	}
	else
		wait(NULL);

	free(args);
	free(path);
}

/**
 * main - Entry point
 *
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	while (1)
	{
		prompt();

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		run_command(line);
	}

	free(line);
	return (0);
}

