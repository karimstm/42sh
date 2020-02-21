/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 14:10:33 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/21 19:07:03 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		init_shell(void)
{
	struct termios	*shell_tmodes;
	int				shell_is_interactive;

	shell_terminal = STDIN_FILENO;
	shell_tmodes = get_termios();
	shell_is_interactive = isatty(shell_terminal);
	if (shell_is_interactive)
	{
		while (ft_tcgetpgrp(shell_terminal) != (g_shell_pgid = getpgrp()))
			kill(-g_shell_pgid, SIGTTIN);
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGCHLD, SIG_DFL);
		g_shell_pgid = getpid();
		if (setpgid(g_shell_pgid, g_shell_pgid) < 0)
		{
			ft_printf_fd(2, "Couldn't put the shell in its own process group");
			exit(EXIT_FAILURE);
		}
		ft_tcsetpgrp(shell_terminal, g_shell_pgid);
		tcgetattr(shell_terminal, shell_tmodes);
	}
}

int			process_status(t_process *p, int status, pid_t pid)
{
	while (p)
	{
		if (p->pid == pid)
		{
			p->status = status;
			JOB_LIST->status = status & 255;
			if (WIFSTOPPED(status))
				p->stopped = 1;
			else
				p->completed = 1;
			return (0);
		}
		p = p->next;
	}
	return (1);
}

int			mark_process_status(t_job_list *jobs, pid_t pid, int status)
{
	t_job		*j;
	t_process	*p;

	if (jobs == NULL)
		return (-1);
	if (pid > 0)
	{
		j = jobs->head;
		while (j)
		{
			p = j->proc_list->head;
			if (!process_status(p, status, pid))
				return (0);
			j = j->next;
		}
		return (-1);
	}
	else if (pid == 0)
		return (-1);
	else
		return (-1);
}

void		update_status(t_job_list *jobs)
{
	int		status;
	pid_t	pid;

	pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);
	if (!mark_process_status(jobs, pid, status))
		update_status(jobs);
}

void		format_job_info(t_job *j)
{
	const char	*sig;
	int			status;

	sig = NULL;
	status = j->proc_list->tail->status;
	if (j->pgid == 0)
		return ;
	if (WIFSTOPPED(status))
		sig = ft_strsignal(WSTOPSIG(status));
	else if (WIFSIGNALED(status))
		sig = ft_strsignal(WTERMSIG(status));
	else if (WEXITSTATUS(status) && status < 127)
		sig = ft_strsignal(WSTOPSIG(status));
	else if (j->kind == J_BACKGROUND)
		sig = "Done";
	if (j->pgid > 0 && sig != NULL)
	{
		ft_printf_fd(STDERR_FILENO, "\n[%d] %ld %s: ",\
			j->pos, (long)j->pgid, sig);
		ft_print_node(j->proc_list->head->node);
		ft_printf("\n");
	}
}
