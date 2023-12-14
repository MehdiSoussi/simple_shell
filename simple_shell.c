#include "main.h"

int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv, __attribute__((unused)) char **envp)
{

	char *buffer = 0;
	size_t buffer_size = 0;
	int x, r, w, i = 0, len = 0, flag_free = 0;
	char **arguments, *command;
	char *env_var_line = 0;
	char *PATH = _getenv(&env_var_line, "PATH");
	char *buffer_error;
	while(1)
	{
		/*Allocating memory for the arguments, "arguments" is an array of strings, the individual arrays should not be allocated as they are automatically allocated by strtok*/
		arguments = malloc(32 * sizeof(char *));
		if(arguments == 0)
			exit(2);
		if(isatty(0))
			write(1, "$ ", 2);
		if (getline(&buffer, &buffer_size, stdin) == -1)
		{
			free(buffer);
			free(arguments);
			free(env_var_line);
			exit(0);
		}
		i = 0;
		arguments[i] = strtok(buffer, " \t\n");
		if(arguments[0] == 0)
		{
			free(arguments);
			continue;
		}
		while (arguments[i])
		{
			i++;	
			arguments[i] = strtok(0, " \t\n");
		}
		if(_strcmp(arguments[0], "exit") == 0)
		{
			free(buffer);
			free(env_var_line);
			free(arguments);
			exit(0);
		}	
		if(_strcmp(arguments[0], "env") == 0)
		{
			free(arguments);
			printenv();
			continue;
		}
		buffer_error = malloc(1024);
		if(buffer_error == 0)
			exit(2);
		len  = handle_path_expetions(&buffer_error, PATH, arguments[0], argv[0]);
		if(len != 0)
		{
			write(2, buffer_error, len);
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
			w = wait(0);
			if(flag_free == 1)
				free(command);
			/*free(arguments);*/
			if(w == -1)
			{
				/*unsure use of free*/
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
	free(env_var_line);
	if(flag_free == 1)
		free(command);
	free(buffer);
	free(arguments);
	return(0);
}

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


char *_getenv(char **env_var_line, char *name)
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
	return 0;
}

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
int _strlen(char *s)
{
	int size = 0;
	while (s[size] != '\0')
		size++;
	return (size);
}
