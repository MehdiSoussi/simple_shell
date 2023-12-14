#include "main.h"

int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv, __attribute__((unused)) char **envp)
{

	char *buffer = 0;
	size_t buffer_size = 0;
	int x, r, w, i = 0;
	char **arguments, *command;
	char *env_var_line = 0;
	char *PATH = _getenv(&env_var_line, "PATH");
	char *buffer_error;
	while(1)
	{
		arguments = malloc(32 * sizeof(char *));
		if(isatty(0))
			write(1, "$ ", 2);
		if (getline(&buffer, &buffer_size, stdin) == -1)
		{
			free(arguments);
			free(env_var_line);
			free(buffer);
			exit(0);
		}
		/*for (i = 0; i < 32; i++)
		  arguments[i] = (char *)malloc(19000);*/
		i = 0;
		arguments[i] = strtok(buffer, " \t\n");
		if(arguments[0] == 0)
			continue;
		while (arguments[i])
		{																	                                	i++;	
			arguments[i] = strtok(0, " \t\n");
		}
		if(_strcmp(arguments[0], "exit") == 0)
		{
			free(arguments);
			free(buffer);
			free(env_var_line);
			exit(0);
		}	
		if(_strcmp(arguments[0], "env") == 0)
		{
			printenv(envp);
			continue;
		}
		buffer_error = malloc(1024);
		handle_path_expetions(&buffer_error, PATH, arguments[0], argv[0]);
		free(buffer_error);
		command = handle_path(arguments[0], PATH, argv[0]);
		if(command == 0)
		{
			perror(argv[0]);
			continue;
		}

		x = fork();
		if (x == -1)
		{
			perror(argv[0]);
			free(env_var_line);
			exit(0);
		}
		if(x != 0)
		{
			w = wait(0);
			free(arguments);
			free(command);
			if(w == -1)
			{
				perror(argv[0]);
				free(env_var_line);
				exit(0);
			}
		}
		else
		{
			r = execve(command, arguments, envp);
			if( r == -1)
			{
				perror(argv[0]);
				free(env_var_line);
				free(arguments);
				free(buffer);
				exit(0);
			}

		}
	}
	free(arguments);
	free(env_var_line);
	free(buffer);
	return(0);
}

void printenv(char **envp)
{
	int i =0;
	extern char **environ;
	(void) envp;
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


void handle_path_expetions(char **buffer_error, char *PATH, char* command, char* shellname)
{
	int len;
       	_strconcat(buffer_error, shellname, ": 1: ");
	_strconcat(buffer_error, *buffer_error, command);
	_strconcat(buffer_error, *buffer_error, ": not found\n");
	len = _strlen(*buffer_error);

	if((PATH == 0 || _strcmp(PATH, "") == 0) && access(command, F_OK) == 0 && command[0] != '/')
	{
		write(2, *buffer_error, len);
		free(*buffer_error);
		exit(127);
	}

	if((PATH == 0 || _strcmp(PATH, "") == 0) && access(command, F_OK) != 0)
	{
		write(2, *buffer_error, len);
		free(*buffer_error);
		exit(127);

	}
}

char *handle_path(char *command, char *PATH, char* shellname)
{
	int i = 0, existence;
	char *token, *path_to_check, *PATH_copied;
	int x, y;
	existence= access(command, F_OK);
	if(existence == 0)
		return command;
	PATH_copied = malloc(1024);
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
	{
		perror(shellname);
		free(PATH_copied);
		free(path_to_check); /*unsure*/
		exit(31);
	}
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
			return path_to_check;
		}
		free(path_to_check);
		token = strtok(0, ":");
	}


	free(PATH_copied);

	return 0;
}
/*char *path_concatenate(char *token, char* command)
{
	int x=0, j=0;
	char *temp = malloc(sizeof(char)*512);
	while(token[x] != '\0')
	{
		temp[y] = token[x];
		x++;
		y++;
	}
	temp[y] = 47;
	y++;
	x = 0;
	while(command[x] != '\0')
	{
		temp[y] = command[x];
		x++;
		y++;
	}
	temp[y] = '\0';
	return temp;


}*/

char *_getenv(char **env_var_line, char *name)
{
	extern char **environ;
	int i = 0;
	char *token;
	int j = 0;

	while(environ[i])
	{
		/*temp = duplicate_string(environ[i]);*/
		j = 0;
		(*env_var_line) = malloc(sizeof(char) * 6000);

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

/*char *duplicate_string(char *environmentvariable)
  {	int j = 0;
  char *temp = malloc(sizeof(char) * 10000);
  while(environmentvariable[j] != '\0')
  {
  temp[j] = environmentvariable[j];
  j++;
  }
  temp[j] = '\0';
  return temp;
  }*/

int _strcmp(char *s1, char *s2)
{
	int size = 0;
	int i = 0;

	while (s1[i] != '\0' || s2[i] != '\0')
	{
		size += (s1[i] - s2[i]);
		if ((s1[i] - s2[i]) != 0)
		{
			break;
		}
		i++;
	}
	return (size);
}
int _strlen(char *s)
{
	int size = 0;

	while (*s != '\0')
	{
		size++;
		s++;
	}
	return (size);
}
