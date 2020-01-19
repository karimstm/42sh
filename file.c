#include <unistd.h>
#include <stdio.h>
#include <string.h>

int			main()
{
	sleep(1);
	pid_t pid = getpid();
	dprintf(2, "\nmy pid is %d and my group is %d\n", pid, getpgid(pid));
	return (0);
}
