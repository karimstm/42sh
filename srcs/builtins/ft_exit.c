
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
	add_to_history(g_token.current, ft_strlen(g_token.current), 0);
	save_history();
	free_history();
	exit(status);
}
