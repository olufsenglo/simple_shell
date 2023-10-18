#include "shell.h"

int status;

int line_num;

char *shell_name;

/**
 * input_san - sanitizes input from the command line
 * @old_buf: buffer to be sanitized
 * @old_size: size of old buffer
 *
 * Return: the new, sanitized buffer
 */
char *input_san(char *old_buf, size_t *old_size)
{
	char *new_buf = malloc(*old_size * 3);
	char *new_ptr = new_buf;
	char *old_ptr = old_buf;

	while (*old_ptr != '\0')
	{
		while (*old_ptr == ' ')
			old_ptr++;
		while (*old_ptr	!= ' ' && *old_ptr != '\0')
		{
			*new_ptr = *old_ptr;
			new_ptr++;
			old_ptr++;
		}
		while (*old_ptr == ' ')
			old_ptr++;
		if (new_ptr != new_buf && *old_ptr != '\0')
		{
			*new_ptr = ' ';
			new_ptr++;
		}

	}
	*new_ptr = '\0';
	free(old_buf);
	return (new_buf);
}

/**
 * err_message - prints error messages and sets status
 * @arg0: command that is causing error
 * @arg1: first argument to command
 */
void err_message(char *arg0, char *arg1)
{
	char err_str_num[20];
	sprintf(err_str_num, "%d", line_num);

	write(STDERR_FILENO, shell_name, _strlen(shell_name));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, err_str_num, _strlen(err_str_num));

	if (str_compare("cd", arg0, MATCH) == TRUE)
	{
		status = 2;
		write(STDERR_FILENO, ": cd: can't cd to ", 18);
		write(STDERR_FILENO, arg1, _strlen(arg1));
		write(STDERR_FILENO, "\n", 1);
		return;
	}

	status = 127;
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, arg0, _strlen(arg0));
	write(STDERR_FILENO, ": not found\n", 12);
}
