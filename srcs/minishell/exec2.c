# include "shell.h"
// to be removed later on
//

void		setup_process(pid_t pgid, t_job_kind kind)
{
	if (shell_is_interactive)
	{
		if (kind == J_FOREGROUND)
			tcsetpgrp(shell_terminal, pgid);
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
	}
}
