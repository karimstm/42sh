/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 18:38:12 by amoutik           #+#    #+#             */
/*   Updated: 2020/03/02 10:34:42 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

t_path		*merge(t_path *first, t_path *second)
{
	t_path		*dummy;
	t_path		*current;
	t_path		*tmp;

	dummy = (t_path *)malloc(sizeof(t_path));
	current = dummy;
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

t_path		*get_mid(t_path *head)
{
	t_path	*fast;
	t_path	*slow;

	if (head == NULL)
		return (head);
	fast = head;
	slow = head;
	while (fast->next != NULL && fast->next->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	return (slow);
}

t_path		*merge_sort(t_path *head)
{
	t_path	*mid;
	t_path	*shalf;

	if (head == NULL || head->next == NULL)
		return (head);
	mid = get_mid(head);
	shalf = mid->next;
	mid->next = NULL;
	return (merge(merge_sort(head), merge_sort(shalf)));
}
