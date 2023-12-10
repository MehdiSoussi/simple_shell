#include "main.h"

int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv, __attribute__((unused)) char **envp)
{

	char *buffer = 0;
	size_t buffer_size = 0;
	int x, r, w, i = 0;
	char **arguments, *command;

	char *PATH = _getenv("PATH");
	while(1)
	{
		if(isatty(0))
			printf("$ ");
		if (getline(&buffer, &buffer_size, stdin) == -1)
			exit(0);
		arguments = malloc(32 * sizeof(char *));
		for (i = 0; i < 32; i++)
			arguments[i] = (char *)malloc(128);
		i = 0;
		arguments[i] = strtok(buffer, " \t\n");
		while (arguments[i])
		{																	                                	i++;	
			arguments[i] = strtok(0, " \t\n");
		}
		if(_strcmp(arguments[0], "exit") == 0)
				exit(0);
		if(_strcmp(arguments[0], "env") == 0)
		{
			printenv(envp);
			continue;
		}
		command = handle_path(arguments[0], PATH);
		if(command == 0)
		{
			perror(argv[0]);
			continue;
		}

		x = fork();
		if (x == -1)
		{
			perror(argv[0]);
			exit(0);
		}
		if(x != 0)
		{
			w = wait(0);
			if(w == -1)
			{
				perror(argv[0]);
				exit(0);
			}
		}
		else
		{
			r = execve(command, arguments, envp);
			if( r == -1)
			{
				perror(argv[0]);
				exit(0);
			}

		}
	}
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
		printf("%s\n", environ[i]);
		i++;
	}
}

char *handle_path(char *command, char *PATH)
{
	int i = 0, existence = access(command, F_OK);
	char *token, *path_to_check, *PATH_copied;

	PATH_copied = malloc(1024);
	while(PATH[i] != 0)
	{
	PATH_copied[i] = PATH[i];
	i++;
	}
	PATH_copied[i] = '\0';
	if(existence == 0)
		return command;
	else
	{
		token = strtok(PATH_copied, ":");
		while(token)
		{
			path_to_check = path_concatenate(token, command);
			existence = access(path_to_check, F_OK);
			if(existence == 0)
				return path_to_check;
			token = strtok(0, ":");
		}
		
	}
	

	return 0;
}
char *path_concatenate(char *token, char* command)
{
	int i=0, j=0;
	char *temp = malloc(sizeof(char)*512);
	while(token[i] != '\0')
	{
		temp[j] = token[i];
		i++;
		j++;
	}
	temp[j] = 47;
	j++;
	i = 0;
	while(command[i] != '\0')
	{
		temp[j] = command[i];
		i++;
		j++;
	}
	temp[j] = '\0';
	return temp;


}

char *_getenv(char *name)
{
	extern char **environ;
	int i;
	char *token;
	char *temp;
	while(environ[i])
	{
		temp = duplicate_string(environ[i]);

		token = strtok(temp, "=");
		if(_strcmp(token, name) == 0)
		{
			token = strtok(0, "=");
			return token;
		}
		i++;
	}
	return 0;
}

char *duplicate_string(char *environmentvariable)
{	int j = 0;
	char *temp = malloc(sizeof(char) * 10000);
	while(environmentvariable[j] != '\0')
	{
		temp[j] = environmentvariable[j];
		j++;
	}
	temp[j] = '\0';
	return temp;
}

int _strcmp(char *s1, char *s2)
{
	int size = 0;

	while (*s1 != '\0' || *s2 != '\0')
	{
		size += (*s1 - *s2);
		if ((*s1 - *s2) != 0)
		{
			break;
		}
		s1++;
		s2++;
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
