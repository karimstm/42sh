# include "shell.h"

int		is_job_stopped(t_job *j)
{
	t_process *p;

	p = j->proc_list->head;
	while (p)
	{
		if (!p->completed && !p->stopped)
			return (0);
		p = p->next;
	}
	return (1);
}

int		is_job_completed(t_job *j)
{
	t_process *p;

	p = j->proc_list->head;
	while (p)
	{
		if (!p->completed)
			return (0);
		p = p->next;
	}
	return (1);
}

void	job_waiting(t_job_list *job_list, t_job *job)
{
	int status;
	pid_t pid;

	pid = waitpid (WAIT_ANY, &status, WUNTRACED);
	if (!mark_process_status (job_list, pid, status)
		&& !is_job_stopped (job)
		&& !is_job_completed (job))
		job_waiting(job_list, job);
}

void	background_job(t_job *job, int cont)
{
	ft_printf_fd(2, "[%d] %lld\n", job->pos, job->pgid);
	if (cont)
		if (kill (-job->pgid, SIGCONT) < 0)
			ft_printf_fd(2, "kill (SIGCONT)");
}

void	foreground_job(t_job_list *job_list, t_job *job, int cont)
{
	ft_tcsetpgrp (shell_terminal, job->pgid);
	if (cont)
	{
		tcsetattr (shell_terminal, TCSADRAIN, &job->tmodes);
		if (kill (- job->pgid, SIGCONT) < 0)
			perror ("kill (SIGCONT)");
	}
	job_waiting (job_list, job);
	ft_tcsetpgrp (shell_terminal, shell_pgid);
	tcgetattr (shell_terminal, &job->tmodes);
	tcsetattr (shell_terminal, TCSADRAIN, get_termios());
}

void		mark_job_as_running (t_job *j)
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

int			get_min_pos(t_job_list *job_list)
{
	t_job	*current;
	int		max;

	max = 0;
	current = (job_list && job_list->node_count)? job_list->head : NULL;
	while (current)
	{
		if (current->pos > max)
			max = current->pos;
		current = current->next;
	}
	return (max);
}
