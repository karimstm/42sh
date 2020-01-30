
#include "shell.h"

int	ft_exit(char **cmds, t_list **env)
{
	int status;

	(void)env;
	status = 0;
	if (cmds[0] && cmds[1])
	{
		return(1);
	}
	if (cmds[0])
		status = ft_atoi(cmds[1]) & 255;
	exit(0);
}
