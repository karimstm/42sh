/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 12:35:21 by amoutik           #+#    #+#             */
/*   Updated: 2019/10/12 10:25:02 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

void		init_s_path(t_list_path	*list)
{
	list->head = NULL;
	list->tail = NULL;
	list->node_count = 0;
}

t_list_path	*alloc_path_list(size_t size)
{
	t_list_path *list;

	if ((list = (t_list_path *)malloc(sizeof(t_list_path) * size)) == NULL)
		return (NULL);
	init_s_path(list);
	return (list);
}

void		push_path(t_list_path *list, char *path, size_t len)
{
	t_path	*node;

	if((node = (t_path *)malloc(sizeof(t_path))) == NULL)
		return ;
	node->path = path;
  	node->path_len = len;
	node->next = NULL;
	if (list->node_count == 0)
	{
		list->head = node;
		list->tail = node;
		node->prev = NULL;
	}
	else
	{
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	list->node_count++;
}

void	free_list_path(t_list_path *list)
{
	t_path	*current;
	t_path	*tmp;

	current = list->head;
	while (current)
	{
		tmp = current->next;
		ft_strdel(&current->path);
		free(current);
		current = tmp;
	}
	init_s_path(list);
}

void	print_list(t_list_path *list)
{
	t_path	*current;

	current = list->head;
	while (current)
	{
		fprintf(stderr, "%s\n", current->path);
		current = current->next;
	}
}

void	delete_target(t_list_path *list, t_path **cu)
{
	t_path	*current;

	current = *cu;
	if (current == list->head)
	{
		list->head = current->next;
		if (current->next)
			current->next->prev = NULL;
		if (current == list->tail)
			list->tail = NULL;
	}
	else if (current == list->tail)
	{
		list->tail = current->prev;
		list->tail->next = NULL;
	}
	else if (current != NULL)
	{
		current->prev->next = current->next;
		current->next->prev = current->prev;
	}
	ft_strdel(&current->path);
	free(current);
	list->node_count--;
}

void    list_func(t_list_path *list, char *s1, char * (f)(char **, char *))
{
    t_path  *current;

    current = list->head;
    while (current)
    {
      current->path = f(&current->path, s1);
      current = current->next;
    }
}
