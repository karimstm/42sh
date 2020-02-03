#include "shell.h"

int		dup3(int oldfd)
{
	if (fcntl(oldfd, F_GETFD) < 0)
		return (-1);
	return (fcntl(oldfd, F_DUPFD, 100));
}
