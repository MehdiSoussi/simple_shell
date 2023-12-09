#include "main.h"
#include <errno.h>

int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv, __attribute__((unused)) char **envp)
{

	char *buffer = 0;
	size_t buffer_size = 0;
	int x, r, w;
	char *token;

		while(1)
		{
			if(isatty(0))
				printf("$ ");
			if (getline(&buffer, &buffer_size, stdin) == -1)
			{
				free(buffer);
			       	exit(1);
			}
			

			x = fork();
			if (x == -1)
			{
				perror(argv[0]);
				free(buffer);
				exit(1);
			}
			if(x != 0)
			{
				w = wait(0);
				if(w == -1)
				{
					perror(argv[0]);
					free(buffer);
					exit(1);
				}
			}
			else
			{

				token = strtok(buffer, " \n");
				r = execve(token, argv, envp);
				if( r == -1)
				{
					perror(argv[0]);
					free(buffer);
					exit(1);
				}
		
			}
		}
	free(buffer);
	return(0);
}
