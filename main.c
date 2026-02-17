#include "main.h"

/**
 * main - Entry point for the shell
 *
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;

	while (1)
	{
		printf("$ ");
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		args = fill_args(line);
		if (args[0] == NULL)
		{
			free(args);
			continue;
		}

		if (_strcmp(args[0], "exit") == 0)
		{
			hsh_exit(args);
			free(args);
			break;
		}

		execute_command(args);
		free(args);
	}

	free(line);
	return (0);
}
