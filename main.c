#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main()
{

	int child = fork();
	if (child == 0)
	{
		char *name = strdup(".fifo42");

		int fd = mkfifo(name, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
		printf("%d\n", fd);
		return (0);
	}
	else
	{
		sleep(3);
		exit(0);
	}
	return (0);
}
