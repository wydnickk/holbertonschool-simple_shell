#include "main.h"

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
 * run_command - execute a single command (no args)
 * @line: command string
 */
void run_command(char *line)
{
	pid_t pid;
	char *args[2];
	char *cmd;

	cmd = trim_spaces(line);
	if (cmd == NULL || *cmd == '\0')
		return;

	args[0] = cmd;
	args[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("Error");
		return;
	}

	if (pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
			perror("./hsh");
		exit(1);
	}
	else
	{
		wait(NULL);
	}
}

/**
 * main - Entry point for simple shell
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
