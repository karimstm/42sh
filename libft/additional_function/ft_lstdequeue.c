/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdequeue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 00:49:40 by aait-ihi          #+#    #+#             */
/*   Updated: 2020/02/06 20:21:17 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdequeue(t_list **list, void (*del)(void **))
{
	t_list	*tmp;

	if (list && *list)
	{
		tmp = (*list)->next;
		if (tmp)
			tmp->tail = (*list)->tail;
		del((void **)&(*list)->content);
		ft_memdel((void **)list);
		*list = tmp;
	}
}
