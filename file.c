#include <unistd.h>
#include <stdio.h>
#include <string.h>

int			main()
{
	dprintf(2, "my pid is %d and my group is %d\n", getpid(), getpgid(getpid()));
	return (0);
}
