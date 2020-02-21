/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 14:10:29 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/21 19:07:23 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		job_waiting(t_job_list *job_list, t_job *job)
{
	int		status;
	pid_t	pid;

	pid = waitpid(WAIT_ANY, &status, WUNTRACED);
	if (!mark_process_status(job_list, pid, status)
		&& !is_job_stopped(job)
		&& !is_job_completed(job))
		job_waiting(job_list, job);
}

void		background_job(t_job *job, int cont)
{
	ft_printf_fd(2, "[%d] %lld\n", job->pos, job->pgid);
	if (cont)
		if (kill(-job->pgid, SIGCONT) < 0)
			ft_printf_fd(2, "kill (SIGCONT)");
}

void		foreground_job(t_job_list *job_list, t_job *job, int cont)
{
	ft_tcsetpgrp(shell_terminal, job->pgid);
	if (cont)
	{
		tcsetattr(shell_terminal, TCSADRAIN, &job->tmodes);
		if (kill(-job->pgid, SIGCONT) < 0)
			ft_printf_fd(2, "kill (SIGCONT) failed");
	}
	job_waiting(job_list, job);
	ft_tcsetpgrp(shell_terminal, g_shell_pgid);
	tcgetattr(shell_terminal, &job->tmodes);
	tcsetattr(shell_terminal, TCSADRAIN, get_termios());
}

void		mark_job_as_running(t_job *j)
{
	t_process *p;

	p = j->proc_list->head;
	while (p)
	{
		p->stopped = 0;
		p = p->next;
	}
	j->notified = 0;
}

void		continue_job(t_job_list *job_list, t_job *j, int foreground)
{
	mark_job_as_running(j);
	if (foreground)
		foreground_job(job_list, j, 1);
	else
	{
		j->kind = J_BACKGROUND;
		background_job(j, 1);
	}
}
