#include "shell.h"

int status;

/**
 * print_env - prints the environment
 *
 * Return: TRUE
 */
int print_env(void)
{
	char **ptr = environ;

	while (*ptr != NULL)
	{
		write(STDOUT_FILENO, *ptr, _strlen(*ptr));
		write(STDOUT_FILENO, "\n", 1);
		ptr++;
	}

	status = 0;

	return (SKIP_FORK);
}
