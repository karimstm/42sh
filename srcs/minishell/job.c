#include "shell.h"


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
		signal (SIGCHLD, SIG_DFL);

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
					if (WIFSTOPPED (status))
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

/* Return true if all processes in the job have stopped or completed.  */
int job_is_stopped (t_job *j)
{
  t_process *p;

  for (p = j->proc_list->head; p; p = p->next)
    if (!p->completed && !p->stopped)
      return 0;
  return 1;
}

/* Return true if all processes in the job have completed.  */
int job_is_completed (t_job *j)
{
  t_process *p;

  for (p = j->proc_list->head; p; p = p->next)
    if (!p->completed)
      return 0;
  return 1;
}


void update_status (t_job_list *jobs)
{
	int status;
	pid_t pid;

	do
		pid = waitpid (WAIT_ANY, &status, WUNTRACED | WNOHANG);
	while (!mark_process_status (jobs, pid, status));
}

void	format_job_info(t_job *j)
{
	const char	*sig;

	if (WIFSTOPPED(j->proc_list->tail->status))
		sig = ft_strsignal(WSTOPSIG(j->proc_list->tail->status));
	else if (WIFSIGNALED(j->proc_list->tail->status))
		sig = ft_strsignal(WTERMSIG(j->proc_list->tail->status));
	else if (WEXITSTATUS(j->proc_list->tail->status))
		sig = ft_strsignal(WSTOPSIG(j->proc_list->tail->status));
	else
		sig = "Done";
	ft_printf_fd(STDERR_FILENO, "\n[%d] %ld %s: ", j->pos, (long)j->pgid, sig);
	ft_print_node(j->proc_list->head->node);
	ft_printf("\n");
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
	free(first);
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
	free(last);
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
		free(current);
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
	max = 0;
	while (current)
	{
		if (current->pos > max)
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
		active->current = CURRENT_PREV;
}

void	set_active_job(t_job_list *jobs, t_job *target)
{
	t_job	*current;
	int		flag;

	flag = 0;
	current = jobs->head;
	while (current)
	{
		if (target && current != target)
		{
			if (current->current == CURRENT_ACTIVE)
			{
				flag = 1;
				current->current = CURRENT_PREV;
			}
		}else if (target == NULL && current->current == CURRENT_PREV)
			current->current = CURRENT_ACTIVE;
		current = current->next;
	}
	if (!flag)
		set_max_as_active(jobs);
}

void	job_notification(t_job_list *jobs)
{
	t_job *current;

	if (jobs == NULL)
		return ;
	current = jobs->head;
	update_status(jobs);
	while (current)
	{
		if (job_is_completed(current))
		{
			if (current->kind == J_BACKGROUND)
				format_job_info(current);
			delete_job(jobs, current);
			current = jobs->head;
			continue;
		}
		else if (job_is_stopped (current) && !current->notified)
		{
			set_active_job(jobs, current);
			format_job_info (current);
			current->current = CURRENT_ACTIVE;
			current->notified = 1;
			current->current = CURRENT_ACTIVE;
		}
		if (current != NULL)
			current = current->next;
	}
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

void	dumpy_process(t_job_list *job_list, t_node *node)
{
	t_list_process *procs;

	procs = (t_list_process *)xmalloc(sizeof(t_list_process));
	init_process_list(procs);
	process_push(procs, 0, NULL, node);
	job_push(job_list, procs, 0);
}
