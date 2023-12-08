#include "main.h"

int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv, __attribute__((unused)) char **envp)
{

	char *buffer = 0;
	size_t buffer_size = 32;
	int x, i = 0, r, w;

		
		while(1)
		{
			x = fork();
			if (x == -1)
			{
				perror("Error:");
				exit(2);
			}
			if(x != 0)
			{
				w = wait(0);
				free(buffer);
				if(w == -1)
				{
					perror("Error:");
					exit(3);
				}
			}
			else
			{
				buffer = malloc(sizeof(char)*buffer_size);
				if(buffer == 0)
					perror("Error:");
				printf("$ ");
				if (getline(&buffer, &buffer_size, stdin) == -1)
					perror("Error:");
				i = 0;
				while(buffer[i] != '\n' && buffer[i] != EOF)
					i++;
				buffer[i] = '\0';
				r = execve(buffer, argv, envp);
				if( r == -1)
				{
					perror(argv[0]);
					exit(4);
				}
		
			}
		}
		
	return(0);
}
