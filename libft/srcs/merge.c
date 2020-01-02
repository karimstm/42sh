/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SortLinkedList.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 18:38:12 by amoutik           #+#    #+#             */
/*   Updated: 2019/04/10 11:44:43 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

t_path *merge(t_path *first, t_path *second)
{
	t_path *dummy;
	t_path *current;
	t_path *tmp;
	
	current = dummy = (t_path *)malloc(sizeof(t_path));
	while (first != NULL && second != NULL)
	{
		if (ft_strcmp(first->path, second->path) <= 0)
		{
			current->next = first;
			first = first->next;
		}
		else
		{
			current->next = second;
			second = second->next;
		}
		current = current->next;
	}
	tmp = dummy->next;
	free(dummy);
	current->next = (first == NULL) ? second : first;
	return (tmp);
}

t_path *get_mid(t_path *head)
{
	t_path *fast;
	t_path *slow;

	if (head == NULL)
		return (head);
	fast = slow = head;
	while (fast->next != NULL && fast->next->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	return (slow);
}

t_path	*merge_sort(t_path *head)
{
	if (head == NULL || head->next == NULL)
		return(head);
	t_path *mid = get_mid(head);
	t_path *shalf = mid->next;
	mid->next = NULL;
	return (merge(merge_sort(head), merge_sort(shalf)));
}
