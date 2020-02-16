/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_job.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 14:10:14 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/15 14:10:15 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
