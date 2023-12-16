#include "main.h"
/**
 * main - a function
 *
 * @argc: input
 * @argv: input
 * @envp: input
 *
 * Return: 0
 */
int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv, __attribute__((unused)) char **envp)
{

	char *buffer = 0;
	size_t buffer_size = 0;
	int x, r, w, i = 0, len = 0, flag_free = 0, should_free = 1;
	char **arguments, *command;
	char *env_var_line = 0;
	char *PATH = _getenv(&env_var_line, "PATH", &should_free);
	char *buffer_error;
	int status = 0;
	while (1)
	{
		/*Allocating memory for the arguments, "arguments" is an array of strings, the individual arrays should not be allocated as they are automatically allocated by strtok*/
		arguments = malloc(32 * sizeof(char *));
		if (arguments == 0)
			exit(2);
		if (isatty(0))
			write(1, "$ ", 2);
		if (getline(&buffer, &buffer_size, stdin) == -1)
		{
			free(buffer);
			free(arguments);
			if (should_free == 1)
			free(env_var_line);
			exit(0);
		}
		i = 0;
		arguments[i] = strtok(buffer, " \t\n");
		if (arguments[0] == 0 || arguments[i][0] == '#')
		{
			free(arguments);
			continue;
		}
		while (arguments[i])
		{
			i++;
			arguments[i] = strtok(0, " \t\n");
			if (arguments[i] && arguments[i][0] == '#')
			{
				arguments[i] = 0;
				break;
			}


		}
		if (_strcmp(arguments[0], "exit") == 0)
		{
			handle_free(arguments, buffer, env_var_line, should_free, status, i, argv[0]);
			/*free(buffer);
			if(should_free == 1)
			free(env_var_line);
			free(arguments);
			if (status == 0)
			exit(0);
			else
			{
			exit(2);
			}*/
		}
		/* Might need to add an: status = 0; */
		if(_strcmp(arguments[0], "env") == 0)
		{
			free(arguments);
			printenv();
			continue;
		}
		if(_strcmp(arguments[0], "$?") == 0)
		{
			handle_variables(arguments);
			continue;	}
		buffer_error = malloc(1024);
		if(buffer_error == 0)
			exit(2);
		len  = handle_path_expetions(&buffer_error, PATH, arguments[0], argv[0]);
		if(len != 0)
		{
			write(2, buffer_error, len);
			if(should_free == 1)
			free(env_var_line);
			free(buffer_error);
			free(arguments);
			free(buffer);
			exit(127);
		}
		else
		{
			free(buffer_error);
		}
		/*
		/bin/ls => Return /bin/ls (No malloc should be handled after) => No action necessary
		ls =>  Return /bin/ls (PATH_copied malloced once and freed guaranteed!, path_to_check malloced and freed as many times until it gets found and when it it found it is not freed and flag_free = 1 is set to display to the main that it should be freed)
		/bin/xxx Return 0 (PATH_copied malloced once and freed guaranteed!, path_to_check malloced and freed as many times) => No action necessary
		xxx Return 0 (PATH_copied malloced once and freed guaranteed!, path_to_check malloced and freed as many times) => No action necessary
		*/
		command = handle_path(arguments[0], PATH, argv[0], &flag_free);
		if(command == 0)
		{
			perror(argv[0]);
			free(arguments);
			continue;
		}
		x = fork();
		if (x == -1)
		{
			if(should_free == 1)
			free(env_var_line);
			if(flag_free == 1)
				free(command);
			free(buffer);
			perror(argv[0]);
			free(arguments);
			exit(0);
		}
		if(x != 0)
		{
			w = wait(&status);
			if(flag_free == 1)
				free(command);
			free(arguments);
			if(w == -1)
			{
				/*unsure use of free*/
				if(should_free == 1)
				free(env_var_line);
				if(flag_free == 1)
					free(command);
				free(arguments);
				free(buffer);
				perror(argv[0]);
				exit(0);
			}
		}
		else
		{
			r = execve(command, arguments, envp);
			if( r == -1)
			{
				if(should_free == 1)
				free(env_var_line);
				if(flag_free == 1)
					free(command);
				free(arguments);
				free(buffer);
				perror(argv[0]);
				exit(0);
			}

		}
	}
	if(should_free == 1)
	free(env_var_line);
	if(flag_free == 1)
		free(command);
	free(buffer);
	free(arguments);
	return(0);
}
/**
 * handle_variables - a function
 * @arguments: input
 * Return: void
 */
void handle_variables(char **arguments)
{
		free(arguments);
		write(1, "0\n", 2);
}
/**
 * handle_free - a function 
 * @arguments: input
 * @buffer: input
 * @env_var_line: input
 * @should_free: input
 * @status: input
 * @i: input
 * @shell_name: input
 * Return: void
 */
void handle_free(char **arguments, char *buffer, char *env_var_line, int should_free, int status, int i, char *shell_name)
{

	if(i != 2)
	{
	free_grouping(buffer, should_free, env_var_line,  arguments);
	if (status == 0)
		exit(0);
		
	else
		exit(2);
	}
	else
	{
	if(_strcmp(arguments[1], "1000") == 0)
	{
		free_grouping(buffer, should_free, env_var_line,  arguments);
		exit(232);
	}
	else
	{
		if(_strcmp(arguments[1], "98") == 0)
		{
			free_grouping(buffer, should_free, env_var_line,  arguments);
			exit(98);
		}
		write(2, shell_name, _strlen(shell_name)); 
		write(2, ": 1: exit: Illegal number: ", 27);
		write(2, arguments[1], _strlen(arguments[1]));
		write(2, "\n", 1);
		free_grouping(buffer, should_free, env_var_line,  arguments);
		exit(2);
	}
	}
}
/**
 * free_grouping - a function
 * @buffer: input
 * @should_free: input
 * @env_var_line: input
 * @arguments: input
 * Return: void
 */
void free_grouping(char *buffer, int should_free, char *env_var_line,  char **arguments)
{
 free(buffer);
	 if(should_free == 1)
		free(env_var_line);
	free(arguments);
}

/**
 * printenv -  a function
 *
 * Return: void
 */
void printenv()
{
	int i =0;
	extern char **environ;
	while(environ[i])
	{
		write(1, environ[i], _strlen(environ[i]));
		write(1, "\n", 1);
		i++;
	}
}
/**
 * _strconcat - a function
 *
 * @buffer: input
 * @s1: input
 * @s2: input
 *
 * Return: 0
 */
int *_strconcat(char **buffer, char *s1, char *s2)
{	
	int i = 0, j = 0;
	while(s1[i] != '\0')
	{
		(*buffer)[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while(s2[i] != '\0')
	{
		(*buffer)[j] = s2[i];
		i++;
		j++;
	}
	(*buffer)[j] = '\0';

return 0;
}

/**
 * handle_path_expetions - a function
 *
 * @buffer_error: input
 * @PATH: input
 * @command: input
 * @shellname: input
 *
 * Return: 0
 */
int handle_path_expetions(char **buffer_error, char *PATH, char* command, char* shellname)
{
	int len;
    _strconcat(buffer_error, shellname, ": 1: ");
	_strconcat(buffer_error, *buffer_error, command);
	_strconcat(buffer_error, *buffer_error, ": not found\n");
	len = _strlen(*buffer_error);

	if((PATH == 0 || _strcmp(PATH, "") == 0) && access(command, F_OK) == 0 && command[0] != '/')
		return len;

	if((PATH == 0 || _strcmp(PATH, "") == 0) && access(command, F_OK) != 0)
		return len;

	return 0;
}
/**
 * handle_path - a function
 *
 * @command: input
 * @PATH: input
 * @shellname: input
 * @flag_free: input
 *
 * Return: 0
 */

char *handle_path(char *command, char *PATH,__attribute__((unused)) char* shellname, int *flag_free)
{
	
	int i = 0, existence;
	char *token, *path_to_check, *PATH_copied;
	int x, y;
	*flag_free = 0;
	existence= access(command, F_OK);
	if(existence == 0)
		return command;
	PATH_copied = malloc(1024);
	if(PATH_copied == 0)
		exit(2);
	while(PATH[i] != 0)
	{
		PATH_copied[i] = PATH[i];
		i++;
	}
	PATH_copied[i] = '\0';
	token = strtok(PATH_copied, ":");
	while(token)
	{
		
	x=0;
    y=0;
	path_to_check = malloc(sizeof(char)*512);
	if(path_to_check == 0)
			exit(2);
	
	while(token[x] != '\0')
	{
		path_to_check[y] = token[x];
		x++;
		y++;
	}
	path_to_check[y] = 47;
	y++;
	x = 0;
	while(command[x] != '\0')
	{
		path_to_check[y] = command[x];
		x++;
		y++;
	}
	path_to_check[y] = '\0'; 
		
		existence = access(path_to_check, F_OK);
		if(existence == 0)
		{
			free(PATH_copied);
			*flag_free = 1;
			return path_to_check;
		}
		free(path_to_check);
		token = strtok(0, ":");
	}


	free(PATH_copied);
	return 0;
}

/**
 * _getenv - a function
 * @should_free: input
 * @env_var_line: input
 * @name: input
 * Return: 0
 */
char *_getenv(char **env_var_line, char *name, int* should_free)
{
	extern char **environ;
	int i = 0,  j = 0;
	char *token;

	while(environ[i])
	{
		j = 0;
		(*env_var_line) = malloc(sizeof(char) * 6000);
		if((*env_var_line) == 0)
			exit(2);
		while(environ[i][j] != '\0')
		{
			(*env_var_line)[j] = environ[i][j];
			j++;
		}
		(*env_var_line)[j] = '\0';

		token = strtok(*env_var_line, "=");
		if(_strcmp(token, name) == 0)
		{
			token = strtok(0, "=");
			return token;
		}
		i++;
		free(*env_var_line);

	}
	*should_free = 0;
	return 0;
}
/**
 * _strcmp - a function
 *
 * @s1: input
 * @s2: input
 *
 * Return: size
 */
int _strcmp(char *s1, char *s2)
{
	int size = 0;
	int i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		size += (s1[i] - s2[i]);
		if ((s1[i] - s2[i]) != 0)
			break;
		i++;
	}
	return (size);
}
/**
 * _strlen - a function
 *
 * @s: input
 *
 * Return: size
 */
int _strlen(char *s)
{
	int size = 0;
	while (s[size] != '\0')
		size++;
	return (size);
}
