#include "shell.h"

int status;

char *shell_name;

/**
 * command_manager - manages the process a command goes through to get executed
 * @args: command and arguments
 *
 * Return: TRUE if success, FALSE if failure
 */
int command_manager(char **args)
{
	int no_err = TRUE;
	char next_op = 'c';
	int what_do;

	if (next_op == 'c')
	{
		what_do = execute_command(args);

		if (what_do == EXIT_SHELL)
			return (EXIT_SHELL);
	}

	if (no_err == FALSE || what_do == FALSE)
		return (FALSE);

	return (TRUE);
}

/**
 * built_ins - checks if a command is a built in
 * @args: command and arguments
 *
 * Return: SKIP_FORK if built in, DO_EXECVE if not a built in, EXIT_SHELL if
 * exit shell, EXIT_SHELL_CODE if exiting with a particular code
 */
int built_ins(char **args)
{
	if (*args == NULL)
		return (SKIP_FORK);

	if (str_compare("exit", *args, MATCH) == TRUE)
		return (EXIT_SHELL);
	else if (str_compare("env", *args, MATCH) == TRUE)
		return (print_env());

	return (DO_EXECVE);
}

/**
 * check_command - checks if a non-built-in exists
 * @args: argument and commands
 *
 * Return: TRUE if valid command, FALSE if not
 */
char *check_command(char **args)
{
	char *command_buf;
	char *full_buf;
	char *path_str = NULL;
	char *path_ptr;
	char *path_tmp;
	char **path_var = NULL;
	char **path_var_ptr;

	if (access(*args, X_OK) == 0)
		return (_strdup(*args));

	if (get_array_element(environ, "PATH=") != NULL)
		path_str = _strdup(get_array_element(environ, "PATH=") + 5);

	path_ptr = path_str;

	if (path_str != NULL)
	{
		if (*path_str == ':')
		{
			command_buf = str_concat("./", *args);
			if (access(command_buf, X_OK) == 0)
			{
				free(path_str);
				return (command_buf);
			}
			else
			{
				free(command_buf);
				path_ptr = _strdup(path_str + 1);
				free(path_str);
				path_str = path_ptr;
			}
		}
		while (*path_ptr != '\0')
		{
			if (*path_ptr == ':' && *(path_ptr + 1) == ':')
			{
				*(path_ptr + 1) = '\0';
				path_tmp = (str_concat(path_str, ".:"));
				path_ptr = str_concat(path_tmp, path_ptr + 2);
				free(path_str);
				free(path_tmp);
				path_str = path_ptr;
			}
			path_ptr++;
		}
	}
	if (path_str != NULL)
		path_var = make_array(path_str, ':', NULL);

	path_var_ptr = path_var;

	command_buf = str_concat("/", *args);

	full_buf = _strdup(command_buf);

	if (path_var != NULL)
	{
		while (*path_var_ptr != NULL && access(full_buf, X_OK) != 0)
		{
			free(full_buf);
			full_buf = str_concat(*path_var_ptr, command_buf);
			path_var_ptr++;
		}
	}

	free(command_buf);
	free(path_str);
	free(path_var);

	if (access(full_buf, X_OK) != 0)
	{
		err_message(args[0], NULL);
		free(full_buf);
		return (NULL);
	}

	return (full_buf);
}

/**
 * execute_command - executes a command
 * @args: command and arguments
 *
 * Return: TRUE or EXIT_SHELL
 */
int execute_command(char **args)
{
	char *buf_ptr = *args;
	char *command_name;
	pid_t pid;
	int what_do = built_ins(args);

	if (what_do == DO_EXECVE)
	{
		command_name = check_command(args);
		if (command_name == NULL)
			return (FALSE);

		pid = fork();
		if (pid == -1)
		{
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			execve(command_name, args, environ);
			exit(EXIT_FAILURE);
		}
		wait(&status);
		free(command_name);
		fflush(stdin);
		if (status != 0)
		status = 2;
	}

	if (str_compare("false", *args, MATCH) == TRUE)
		status = 1;

	if (*args != buf_ptr)
		free(*args);
	args++;

	while (*args != NULL)
	{
		while (*buf_ptr != '\0')
		{
			buf_ptr++;
		}
		buf_ptr++;

		if (*args != buf_ptr)
			free(*args);

		args++;
	}

	if (what_do == EXIT_SHELL)
		return (EXIT_SHELL);

	if (status != 0)
		return (FALSE);

	return (TRUE);
}
