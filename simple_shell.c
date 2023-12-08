#include "main.h"

void main (void)
{
	char *buffer = 0;
	size_t buffer_size = 32;
	ssize_t return_value;
	char *argv[] = {".", 0};
	int res;
	int x;

	while(1)
	{
	x = fork();
	if(x != 0)
		wait(0);
	else
	{
		buffer = malloc(sizeof(char)*buffer_size);
		printf("$ ");
		return_value = getline(&buffer, &buffer_size, stdin);
		printf("\n");
		execve(buffer, argv, 0 );
		
	}
	}
}
