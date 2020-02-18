/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 14:09:57 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/15 14:29:11 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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

void	print_and_delete_job(t_job_list *jobs, t_job **job)
{
	t_job	*current;

	current = *job;
	if (current->kind == J_FOREGROUND || current->kind == J_NON_INTERACTIVE)
	{
		if (current->proc_list && current->proc_list->tail)
			jobs->status = current->proc_list->tail->status;
	}
	format_job_info(current);
	delete_job(jobs, current);
	*job = jobs->head;
}

void	set_job_as_notified(t_job_list *jobs, t_job *current)
{
	format_job_info(current);
	set_active_job2(jobs, current);
	current->current = CURRENT_ACTIVE;
	current->notified = 1;
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
			print_and_delete_job(jobs, &current);
			continue;
		}
		else if (is_job_stopped(current) && !current->notified)
			set_job_as_notified(jobs, current);
		if (current->notified)
			stopped_once = 1;
		if (current != NULL)
			current = current->next;
	}
	!stopped_once ? set_max_as_active(jobs) : 0;
}

void	dummy_process(t_job_list *job_list, t_node *node, t_job_kind kind)
{
	t_list_process *procs;

	procs = (t_list_process *)xmalloc(sizeof(t_list_process));
	init_process_list(procs);
	process_push(procs, 0, node);
	job_push(job_list, procs, 0);
	job_list->tail->kind = kind;
}
