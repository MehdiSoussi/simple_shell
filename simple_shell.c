#include "main.h"

int main (void)
{
	char *buffer = 0;
	size_t buffer_size = 32;
	char *argv[] = {".", 0};
	int x;

	while(1)
	{
	x = fork();
	if(x != 0)
	{
		wait(0);
	return(0);
	}
	else
	{
		buffer = malloc(sizeof(char)*buffer_size);
		printf("$ ");
		getline(&buffer, &buffer_size, stdin);
		printf("\n");
		execve(buffer, argv, 0 );
		
	}
	}
	return(0);
}
