/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expans.utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 11:39:20 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/25 11:56:43 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			free_simple_cmd(t_simple_command *job)
{
	free(job);
	job = NULL;
}

void	delete_first_cmd(t_list_simple_command *list)
{
	t_simple_command *first;

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
	free_simple_cmd(first);
	list->node_count--;
}

void	delete_last_cmd(t_list_simple_command *list)
{
	t_simple_command	*current;
	t_simple_command	*last;

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
	free_simple_cmd(last);
	list->node_count--;
}

void	delete_simple_cmd(t_list_simple_command *list,
					t_simple_command *target, t_simple_command *m_prev)
{
	t_simple_command *current;
	t_simple_command *prev;

	prev = m_prev;
	current = target;
	if (current == list->head)
		delete_first_cmd(list);
	else if (current == list->tail)
		delete_last_cmd(list);
	else
	{
		prev->next = current->next;
		free_simple_cmd(current);
		list->node_count--;
	}
}

void		clean_list(t_list_simple_command	*list)
{
	t_simple_command	*current;
	t_simple_command	*prev;

	if (list == NULL)
		return ;
	current = list->head;
	prev = NULL;
	while (current)
	{
		if (current->name == NULL)
		{
			delete_simple_cmd(list, current, prev);
			prev = NULL;
			current = list->head;
			continue ;
		}
		prev = current;
		current = current->next;
	}
}
