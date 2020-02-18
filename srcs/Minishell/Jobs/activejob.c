/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activejob.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 14:09:40 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/15 14:21:17 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			free_job(t_job *job)
{
	free_proc(&job->proc_list);
	free(job);
	job = NULL;
}

void			set_max_as_active(t_job_list *jobs)
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

t_job			*get_prev_stopped(t_job_list *list)
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

t_job			*notified_jobs(t_job_list *jobs)
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

void			set_active_job2(t_job_list *jobs, t_job *target)
{
	t_job	*current;

	if ((current = get_prev_stopped(jobs)) && current != target)
		current->current = CURRENT_PREV;
	else if (notified_jobs(jobs) == NULL)
		set_max_as_active(jobs);
}
