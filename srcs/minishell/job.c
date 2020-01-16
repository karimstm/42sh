#include "shell.h"


t_job_list	*job_list(void)
{
	static t_job_list jobs;
	return (&jobs);
}

t_job_list	*get_job_list(t_job_list *jobs)
{
	static t_job_list *list = NULL;
	if (jobs != NULL)
		list = jobs;
	return (list);
}

/*
** Default implementation provided by GNU tutorial
*/

void      	change(int sig)
{
	printf("get changes in child\n");
}

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


void	launch_process(t_process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground, char **env)
{
	pid_t pid;

	if (shell_is_interactive)
	{
		pid = getpid();
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
	execve(p->argv[0], p->argv, env);
	syntax_error("failed to execute the command");
	exit(1);
}

/* Put job j in the foreground.  If cont is nonzero,
   restore the saved terminal modes and send the process group a
   SIGCONT signal to wake it up before we block.  */

void put_job_in_foreground (t_job *j, int cont)
{
  /* Put the job into the foreground.  */
  tcsetpgrp (shell_terminal, j->pgid);

  /* Send the job a continue signal, if necessary.  */
  if (cont)
    {
      tcsetattr (shell_terminal, TCSADRAIN, &j->tmodes);
      if (kill (- j->pgid, SIGCONT) < 0)
        perror ("kill (SIGCONT)");
    }

  /* Wait for it to report.  */
  wait_for_job (j);

  /* Put the shell back in the foreground.  */
  tcsetpgrp (shell_terminal, shell_pgid);

  /* Restore the shell’s terminal modes.  */
  tcgetattr (shell_terminal, &j->tmodes);
  tcsetattr (shell_terminal, TCSADRAIN, &shell_tmodes);
}


void put_job_in_background (t_job *j, int cont)
{

	t_job_list *list = get_job_list(NULL);
	ft_printf_fd(2, "[%d] %lld\n", list->node_count, j->pgid);
	/* Send the job a continue signal, if necessary.  */
	if (cont)
		if (kill (-j->pgid, SIGCONT) < 0)
			perror ("kill (SIGCONT)");
}

int		mark_process_status(t_job_list *jobs ,pid_t pid, int status)
{
	t_job *j;
	t_process *p;

	if (jobs == NULL)
		return (-1);
	 if (pid > 0)
    {
      /* Update the record for the process.  */
		for (j = jobs->head; j; j = j->next)
			for (p = j->proc_list->head; p; p = p->next)
				if (p->pid == pid)
				{
					p->status = status;
					if (WIFSTOPPED (status))
						p->stopped = 1;
					else
					{
						p->completed = 1;
						if (WIFSIGNALED (status))
						ft_printf_fd (STDERR_FILENO, "%d: Terminated by signal %d.\n",
									(int) pid, WTERMSIG (p->status));
					}
					return 0;
				}
		return -1;
    } else if (pid == 0 || errno == ECHILD)
		/* No processes ready to report */
		return (-1);
	else {
		/* Other weird errors.  */
		perror ("waitpid");
		return -1;
	}
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

void wait_for_job (t_job *j)
{
  int status;
  pid_t pid;

	t_job_list *jobs = get_job_list(NULL);
  do
    pid = waitpid (WAIT_ANY, &status, WUNTRACED);
  while (!mark_process_status (jobs, pid, status)
         && !job_is_stopped (j)
         && !job_is_completed (j));
}

void update_status (void)
{
  int status;
  pid_t pid;

	t_job_list *jobs = get_job_list(NULL);
	do
		pid = waitpid (WAIT_ANY, &status, WUNTRACED | WNOHANG);
	while (!mark_process_status (jobs, pid, status));
}

void	format_job_info (t_job *j, const char *status)
{
  ft_printf_fd(STDERR_FILENO, "%ld (%s): %s\n", (long)j->pgid, status, j->command);
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

void	job_notification(t_job_list *jobs)
{
	t_job *current;

	if (jobs == NULL)
		return ;
	current = jobs->head;
	update_status ();
	while (current)
	{
		if (job_is_completed(current))
		{
			format_job_info(current, "Completed");
			delete_job(jobs, current);
			current = jobs->head;
			continue;
		}
		else if (job_is_stopped (current) && !current->notified)
		{
			format_job_info (current, "stopped");
				current->notified = 1;
				//jlast = j;
		}
		if (current != NULL)
			current = current->next;
	}
}

void	dumpy_process(t_job_list *job_list, t_node *node)
{
	t_list_process *procs;

	procs = (t_list_process *)xmalloc(sizeof(t_list_process));
	init_process_list(procs);
	process_push(procs, 0, NULL, node);
	job_push(job_list, procs, 0);
}
