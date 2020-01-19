
#include "shell.h"

int		array_len(char **cmds)
{
	int count;

	count = 0;
	while (cmds[count])
		count++;
	return (count);
}

void		ft_exit(char **cmds, t_list **env)
{
	// Needs better implementation: and that's for later on.
	int array_size;

	array_size = array_len(cmds);
	if (array_size <= 1)
	{
		(void)env;
		exit(0);
	}
}
