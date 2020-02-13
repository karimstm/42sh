
#include "shell.h"

int	ft_exit(char **cmds)
{
	int status;

	status = 0;
	if (cmds[0] && cmds[1])
	{
		return(1);
	}
	if (cmds[0])
		status = ft_atoi(cmds[1]) & 255;
	// i need maybe to save and free history in every exit in the shell
	save_history();
	free_history();
	exit(status);
}
