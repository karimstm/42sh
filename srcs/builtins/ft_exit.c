
#include "shell.h"

int	ft_exit(char **cmds)
{
	int status;

	status = 0;
	if (cmds[1] && cmds[2])
	{
		return(1);
	}
	if (cmds[1])
		status = ft_atoi(cmds[1]) & 255;
	exit(0);
}
