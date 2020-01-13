#include "shell.h"


t_job_list	*job_list(void)
{
	static t_job_list jobs;
	return (&jobs);
}

/*
** Default implementation provided by GNU tutorial
*/

void		init_shell()
{
	/* see if we are running interactively */
	shell_terminal = STDIN_FILENO;
	shell_is_interactive = isatty(shell_terminal);

	if (shell_is_interactive)
	{
		/* loop until we are in the forground */
		while (tcgetpgrp(shell_terminal) != (shell_pgid =  getpgrp()))
			kill (-shell_pgid, SIGTTIN);
		/* Ignore interactive and job-control signals.  */
		//signal (SIGINT, sig_handler);
		signal (SIGINT, SIG_IGN);
		signal (SIGQUIT, SIG_IGN);
		signal (SIGTSTP, SIG_IGN);
		signal (SIGTTIN, SIG_IGN);
		signal (SIGTTOU, SIG_IGN);
		signal (SIGCHLD, SIG_IGN);

		/* Put ourselves in our own process group */
		shell_pgid = getpid();
		if (setpgid (shell_pgid, shell_pgid) < 0)
		{
			ft_printf_fd(2, "Couldn't put the shell in its own process group");
			exit(EXIT_FAILURE);
		}
		/* Grab control of the terminal. */
		tcsetpgrp (shell_terminal, shell_pgid);

		/* Save default terminal attributes for shell */
		tcgetattr(shell_terminal, &shell_tmodes);
	}
}


void	launch_process(char **p, pid_t pgid, int infile, int outfile, int errfile, int foreground, char **env)
{
	pid_t pid;

	if (shell_is_interactive)
	{
		pid = getpid();
		ft_printf_fd(2, "should be here only one %lld\n", pid);
		pgid = (pgid == 0) ? pid : pgid;
		setpgid(pid, pgid);
		if (foreground)
			tcsetpgrp(shell_terminal, pgid);
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
	}
	if (infile != STDIN_FILENO)
	{
		dup2(infile, STDIN_FILENO);
		close(infile);
	}
	if (outfile != STDOUT_FILENO)
	{
		dup2 (errfile, STDERR_FILENO);
      	close (errfile);
	}
	execve(p[0], p, env);
	syntax_error("failed to execute the command");
	exit(1);
}

