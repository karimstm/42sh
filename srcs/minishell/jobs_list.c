# include "jobs.h"

void	init_job_list(t_job_list *jobs)
{
	jobs->head = NULL;
	jobs->tail = NULL;
	jobs->node_count = 0;
	jobs->status = 0;
}


void		job_push(t_job_list *jobs, t_list_process *p, pid_t pgid)
{
	t_job *job;

	job = (t_job *)xmalloc(sizeof(t_job));
	job->pgid = pgid;
	job->proc_list = p;
	job->kind = J_FOREGROUND;
	job->command = NULL;
	job->pos = 0;
	job->current = CURRENT_NONE;
	job->stdin = 0;
	job->stdout = 1;
	job->stderr = 2;
	job->next = NULL;
	if (jobs->node_count == 0)
		jobs->head = job;
	else
		jobs->tail->next = job;
	jobs->tail = job;
	jobs->node_count++;
}


void		init_process_list(t_list_process *p)
{
	p->head = NULL;
	p->tail = NULL;
	p->node_count = 0;
}

void		process_push(t_list_process *list, pid_t pid, char **args, t_node *node)
{
	t_process *p;

	p = (t_process *)xmalloc(sizeof(t_process));
	p->completed = 0;
	p->stopped = 0;
	p->status = 0;
	p->pid = pid;
	p->argv = args;
	p->node = node;
	p->next = NULL;
	if (list->node_count == 0 || list->head == NULL)
		list->head = p;
	else
		list->tail->next = p;
	list->tail = p;
	list->node_count++;
}
