#include "shell.h"

int			ft_tcsetpgrp(int fd, pid_t pgrp_id)
{
	return ioctl(fd, TIOCSPGRP, &pgrp_id);
}

pid_t		ft_tcgetpgrp(int fd)
{
	int pgrp;

	if (ioctl(fd, TIOCGPGRP, &pgrp) < 0)
		return ((pid_t)-1);
	return ((pid_t)pgrp);
}

void		init_shell()
{
	struct termios	*shell_tmodes;
	int				shell_is_interactive;

	shell_terminal = STDIN_FILENO;
	shell_tmodes = get_termios();
	shell_is_interactive = isatty(shell_terminal);
	if (shell_is_interactive)
	{
		while (ft_tcgetpgrp(shell_terminal) != (shell_pgid =  getpgrp()))
			kill (-shell_pgid, SIGTTIN);
		signal (SIGINT, sig_handler);
		signal (SIGQUIT, SIG_IGN);
		signal (SIGTSTP, SIG_IGN);
		signal (SIGTTIN, SIG_IGN);
		signal (SIGTTOU, SIG_IGN);
		signal (SIGCHLD, SIG_DFL);
		shell_pgid = getpid();
		if (setpgid (shell_pgid, shell_pgid) < 0)
		{
			ft_printf_fd(2, "Couldn't put the shell in its own process group");
			exit(EXIT_FAILURE);
		}
		ft_tcsetpgrp (shell_terminal, shell_pgid);
		tcgetattr(shell_terminal, shell_tmodes);
	}
}



int		mark_process_status(t_job_list *jobs, pid_t pid, int status)
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
			while (p)
			{
				if (p->pid == pid)
				{
					p->status = status;
					jobs->status = status & 255;
					if (WIFSTOPPED(status))
						p->stopped = 1;
					else
						p->completed = 1;
					return (0);
				}
				p = p->next;
			}
			j = j->next;
		}
		return (-1);
	}
	else if (pid == 0)
		return (-1);
	else
		return (-1);
}

void update_status (t_job_list *jobs)
{
	int status;
	pid_t pid;

	pid = waitpid (WAIT_ANY, &status, WUNTRACED | WNOHANG);
	if (!mark_process_status (jobs, pid, status))
		update_status(jobs);
}

void	format_job_info(t_job *j)
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
		ft_printf_fd(STDERR_FILENO, "\n[%d] %ld %s: ", j->pos, (long)j->pgid, sig);
		ft_print_node(j->proc_list->head->node);
		ft_printf("\n");
	}
}

void	free_proc(t_list_process **proces)
{
	t_process	*current;
	t_process	*next;

	current = (*proces)->head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	free(*proces);
	*proces = NULL;
}

void	free_job(t_job *job)
{
	free_proc(&job->proc_list);
	free(job);
	job = NULL;
}

void	free_job_list(t_job_list *jobs)
{
	t_job *job;

	job = jobs->head;
	while (job)
	{
		free_job(job);
		job = NULL;
		job = job->next;
	}
	free(jobs);
}

t_job	*find_job(t_job_list *list, t_job *job, t_job **prev)
{
	t_job *current;

	current = list->head;
	*prev = NULL;
	while (current)
	{
		if (current == job)
			return (job);
		*prev = current;
		current = current->next;
	}
	return (current);
}

void	delete_first(t_job_list *list)
{
	t_job *first;

	if (list->node_count == 0)
		return ;
	first = list->head;
	if (list->node_count == 1)
	{
		list->head = NULL;
		list->tail = NULL;
	}
	else
		list->head = first->next;
	free_job(first);
	list->node_count--;
}

void	delete_last(t_job_list *list)
{
	t_job *current;
	t_job *last;

	if (list->node_count == 0)
		return ;
	current = list->head;
	last = list->tail;
	if (list->node_count == 1)
	{
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		while (current->next != list->tail)
			current = current->next;
		list->tail = current;
		list->tail->next = NULL;
	}
	free_job(last);
	list->node_count--;
}

void	delete_job(t_job_list *list, t_job *target)
{
	t_job *current;
	t_job *prev;

	prev = NULL;
	current = NULL;
	current = find_job(list, target, &prev);
	if (current == list->head)
		delete_first(list);
	else if (current == list->tail)
		delete_last(list);
	else
	{
		prev->next = current->next;
		free_job(current);
		list->node_count--;
	}
}

void	set_max_as_active(t_job_list *jobs)
{
	t_job	*current;
	t_job	*prev;
	t_job	*active;
	int		max;

	current = jobs->head;
	active = NULL;
	prev = NULL;
	max = 0;
	while (current)
	{
		if (current->pos > max && current->pgid > 0)
		{
			max = current->pos;
			prev = active;
			active = current;
		}

		current = current->next;
	}
	if (active)
		active->current = CURRENT_ACTIVE;
	if (prev)
		prev->current = CURRENT_PREV;
}

t_job		*get_prev_stopped(t_job_list *list)
{
	t_job	*current;

	current = list->head;
	while (current)
	{
		if (current->current == CURRENT_ACTIVE)
			return (current);
		current->current = CURRENT_NONE;
		current = current->next;
	}
	return (NULL);
}

t_job	*notified_jobs(t_job_list *jobs)
{
	t_job *current;

	current = jobs->head;
	while (current)
	{
		if (current->notified)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	set_active_job2(t_job_list *jobs, t_job *target)
{
	t_job	*current;

	if ((current = get_prev_stopped(jobs)) && current != target)
		current->current = CURRENT_PREV;
	else if (notified_jobs(jobs) == NULL)
		set_max_as_active(jobs);
}

int		is_job_signaled(t_job *j)
{
	t_process *p;

	p = j->proc_list->head;
	while (p)
	{
		if (!p->signaled)
			return (0);
		p = p->next;
	}
	return (1);
}

void	job_notification(t_job_list *jobs)
{
	t_job	*current;
	int		stopped_once;

	if (jobs == NULL)
		return ;
	current = jobs->head;
	stopped_once = 0;
	update_status(jobs);
	while (current)
	{
		if (is_job_completed(current))
		{
			if(current->kind == J_FOREGROUND || current->kind == J_NON_INTERACTIVE)
			{
				if (current->proc_list && current->proc_list->tail)
					jobs->status = current->proc_list->tail->status;
			}
			format_job_info(current);
			delete_job(jobs, current);
			current = jobs->head;
			continue;
		}
		else if (is_job_stopped(current) && !current->notified)
		{
			format_job_info (current);
			set_active_job2(jobs, current);
			current->current = CURRENT_ACTIVE;
			current->notified = 1;
		}
		if (current->notified)
			stopped_once = 1;
		if (current != NULL)
			current = current->next;
	}
	if (!stopped_once)
		set_max_as_active(jobs);
}

void	dummy_process(t_job_list *job_list, t_node *node, t_job_kind kind)
{
	t_list_process *procs;

	procs = (t_list_process *)xmalloc(sizeof(t_list_process));
	init_process_list(procs);
	process_push(procs, 0, NULL, node);
	job_push(job_list, procs, 0);
	job_list->tail->kind = kind;
}
