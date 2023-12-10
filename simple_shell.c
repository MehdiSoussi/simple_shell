#include "main.h"

int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv, __attribute__((unused)) char **envp)
{

	char *buffer = 0;
	size_t buffer_size = 0;
	int x, r, w, i = 0;
	char **arguments;

	while(1)
	{
		if(isatty(0))
			printf("$ ");
		if (getline(&buffer, &buffer_size, stdin) == -1)
			exit(0);


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

			arguments = malloc(32 * sizeof(char *));
			for (i = 0; i < 32; ++i)
				arguments[i] = (char *)malloc(128);
			i = 0;
			arguments[i] = strtok(buffer, " \t\n");
			while (arguments[i])
			{
				i++;
				arguments[i] = strtok(0, " \t\n");
			}
			r = execve(arguments[0], arguments, envp);
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
